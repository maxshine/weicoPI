#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "constants.h"
#include "debug_util.h"
#include "datatype.h"
#include "http_util.h"

void free_http_request(PTR_HTTP_REQUEST request)
{
  if (request != NULL) {
    if (request->headers != NULL) {
      free(request->headers);
      request->headers = NULL;
    }
    if (request->body != NULL) {
      free(request->body);
      request->body = NULL;
    }
    if (request->form != NULL) {
      free(request->form);
      request->form = NULL;
    }
    free(request);
  }
}

void free_http_response(PTR_HTTP_RESPONSE response)
{
  if (response != NULL) {
    if (response->headers != NULL) {
      free(response->headers);
      response->headers = NULL;
    }
    if (response->body != NULL) {
      free(response->body);
      response->body = NULL;
    }
    if (response->error != NULL) {
      free(response->error);
      response->error = NULL;
    }
    free(response);
  }
}

PTR_HTTP_REQUEST alloc_http_request(uint32_t headers_qty, uint32_t form_length, uint32_t body_length)
{
  uint32_t i = 0;
  PTR_HTTP_REQUEST request = (PTR_HTTP_REQUEST) malloc(sizeof(HTTP_REQUEST));

  if (request != NULL) {
    if (request->headers_qty > 0) {
      request->headers = (PTR_HTTP_HEADER) malloc(sizeof(HTTP_HEADER)*headers_qty);
    } else {
      request->headers = NULL;
      request->headers_qty = 0;
    }
    if (request->headers != NULL) {
      for(i = 0; i<headers_qty; i++) {
	request->headers[i].name = NULL;
	request->headers[i].value = NULL;
      }
      request->headers_qty = headers_qty;
    } else {
      free(request);
      return NULL;
    }
    if (form_length > 0) {
      request->form = (PTR_HTTP_FORM) malloc(sizeof(HTTP_FORM)*form_length);
    } else {
      request->form = NULL;
      request->form_length = 0;
    }
    if (request->form != NULL) {
      for(i = 0; i<form_length; i++) {
        request->form[i].name = NULL;
        request->form[i].value = NULL;
      }
      request->form_length = form_length;
    } else {
      free(request->headers);
      free(request);
      return NULL;
    }
    if (body_length > 0) {
      request->body = (void*) malloc(sizeof(body_length));
    } else {
      request->body = NULL;
      request->body_length = 0;
    }
    if (request->body != NULL) {
      request->body_length = body_length;
      memset(request->body, '\0', body_length);
    } else {
      free(request->form);
      free(request->headers);
      free(request);
      return NULL;
    }
  }
  return request;
}

PTR_HTTP_RESPONSE alloc_http_response(uint32_t headers_qty, uint32_t body_length)
{
  uint32_t i = 0;
  PTR_HTTP_RESPONSE response = (PTR_HTTP_RESPONSE) malloc(sizeof(HTTP_RESPONSE));

  if (response != NULL) {
    if (headers_qty > 0) {
      response->headers = (PTR_HTTP_HEADER) malloc(sizeof(HTTP_HEADER)*headers_qty);
    } else {
      response->headers = NULL;
      response->headers_qty = 0;
    }
    if (response->headers != NULL) {
      for(i = 0; i<headers_qty; i++) {
	response->headers[i].name = NULL;
	response->headers[i].value = NULL;
      }
      response->headers_qty = headers_qty;
    } else {
      free(response);
      return NULL;
    }
    if (body_length > 0) {
      response->body = (void*) malloc(body_length);
    } else {
      response->body = NULL;
      response->body_length = 0;
    }
    if (response->body != NULL) {
      response->body_length = body_length;
      memset(response->body, '\0', body_length);
    } else {
      free(response->headers);
      free(response);
      return NULL;
    }
    response->error = (char*) malloc(sizeof(char)*CURL_ERROR_SIZE);
    if (response->error != NULL) {
      memset(response->error, '\0', CURL_ERROR_SIZE*sizeof(char));
    } else {
      free(response->body);
      free(response->headers);
      free(response);
      return NULL;
    }
  }
  return response;
}

PTR_HTTP_RESPONSE http_get(char* url, PTR_HTTP_REQUEST request)
{
  int i = 0;
  char* sp = (char*) malloc(1024*sizeof(char));
  memset(sp, '\0', 1024*sizeof(char));
  CURL* curl_handler = NULL;
  CURLcode curl_perform_ret;
  struct curl_slist* slist = NULL;
  PTR_HTTP_RESPONSE response = alloc_http_response(0, 1024);

  curl_handler = curl_easy_init();
  curl_easy_setopt(curl_handler, CURLOPT_HTTPGET, 1L);
  
  for (i=0; i<request->headers_qty; i++) {
    sprintf(sp, "%s: %s", request->headers[i].name, request->headers[i].value);
    curl_slist_append(slist, sp);
    memset(sp, '\0', 1024*sizeof(char));
  }

  if (request->form_length != 0) {
    if (strchr(url, '?') == NULL) {
      sprintf(sp, "%s?", url);
    } else {
      sprintf(sp, "%s", url);
    }
    for (i=0; i<request->form_length; i++) {
      if (i == 0 && strchr(url, '?') == NULL) {
	sprintf(sp, "%s%s=%s", sp, request->form[i].name, request->form[i].value);
      } else {
	sprintf(sp, "%s&%s=%s", sp, request->form[i].name, request->form[i].value);
      }
    }
  } else {
    sprintf(sp, "%s", url);
  }
  curl_easy_setopt(curl_handler, CURLOPT_URL, sp);
  curl_easy_setopt(curl_handler, CURLOPT_HTTPHEADER, slist);
  curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, write_body);
  curl_easy_setopt(curl_handler, CURLOPT_WRITEDATA, response);
  curl_easy_setopt(curl_handler, CURLOPT_HEADERFUNCTION, parse_header);
  curl_easy_setopt(curl_handler, CURLOPT_HEADERDATA, response);
  curl_perform_ret = curl_easy_perform(curl_handler);
  curl_easy_getinfo(curl_handler, CURLINFO_RESPONSE_CODE, &(response->status_code));
  curl_easy_cleanup(curl_handler);
  curl_slist_free_all(slist);
  free(sp);
  return response;
}

PTR_HTTP_RESPONSE http_post(char* url, PTR_HTTP_REQUEST request)
{
  int i = 0;
  char* sp = (char*) malloc(1024*sizeof(char));
  memset(sp, '\0', 1024*sizeof(char));
  CURL* curl_handler = NULL;
  CURLcode curl_perform_ret;
  struct curl_slist* slist = NULL;
  struct curl_httppost* post = NULL;
  struct curl_httppost* last = NULL;
  PTR_HTTP_RESPONSE response = alloc_http_response(0, 1024);

  curl_handler = curl_easy_init();
  curl_easy_setopt(curl_handler, CURLOPT_URL, url);
  curl_easy_setopt(curl_handler, CURLOPT_POST, 1L);

  for (i=0; i<request->headers_qty; i++) {
    sprintf(sp, "%s: %s", request->headers[i].name, request->headers[i].value);
    curl_slist_append(slist, sp);
    memset(sp, '\0', 1024*sizeof(char));
  }
  curl_easy_setopt(curl_handler, CURLOPT_HTTPHEADER, slist);

  if (request->form_length == 0) {
    curl_easy_setopt(curl_handler, CURLOPT_POSTFIELDS, (void*)(request->body));
    curl_easy_setopt(curl_handler, CURLOPT_POSTFIELDSIZE, request->body_length);
  } else {
    for (i=0; i<request->form_length; i++) {
      curl_formadd(&post, &last, CURLFORM_PTRNAME, request->form[i].name, CURLFORM_PTRCONTENTS, request->form[i].value, CURLFORM_END);
    }
    curl_easy_setopt(curl_handler, CURLOPT_HTTPPOST, post);
  }
  curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, write_body);
  curl_easy_setopt(curl_handler, CURLOPT_WRITEDATA, response);
  curl_easy_setopt(curl_handler, CURLOPT_HEADERFUNCTION, parse_header);
  curl_easy_setopt(curl_handler, CURLOPT_HEADERDATA, response);
  curl_perform_ret = curl_easy_perform(curl_handler);
  curl_easy_getinfo(curl_handler, CURLINFO_RESPONSE_CODE, &(response->status_code));
  curl_easy_cleanup(curl_handler);
  curl_slist_free_all(slist);
  free(sp);
  return response;
}

PTR_HTTP_RESPONSE https_get(char* url, PTR_HTTP_REQUEST request)
{
  int i = 0;
  char* sp = (char*) malloc(1024*sizeof(char));
  memset(sp, '\0', 1024*sizeof(char));
  CURL* curl_handler = NULL;
  CURLcode curl_perform_ret;
  struct curl_slist* slist = NULL;
  PTR_HTTP_RESPONSE response = alloc_http_response(0, 1024);

  curl_handler = curl_easy_init();
  curl_easy_setopt(curl_handler, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl_handler, CURLOPT_USE_SSL, CURLUSESSL_TRY);
  curl_easy_setopt(curl_handler, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl_handler, CURLOPT_SSL_VERIFYHOST, 0L);

  for (i=0; i<request->headers_qty; i++) {
    sprintf(sp, "%s: %s", request->headers[i].name, request->headers[i].value);
    curl_slist_append(slist, sp);
    memset(sp, '\0', 1024*sizeof(char));
  }
  if (request->form_length != 0) {
    if (strchr(url, '?') == NULL) {
      sprintf(sp, "%s?", url);
    } else {
      sprintf(sp, "%s", url);
    }   
    for (i=0; i<request->form_length; i++) {
      if (i == 0 && strchr(url, '?') == NULL) {
        sprintf(sp, "%s%s=%s", sp, request->form[i].name, request->form[i].value);
      } else {
        sprintf(sp, "%s&%s=%s", sp, request->form[i].name, request->form[i].value);
      }
    }
  } else {
    sprintf(sp, "%s", url);
  }
  curl_easy_setopt(curl_handler, CURLOPT_URL, sp);
  curl_easy_setopt(curl_handler, CURLOPT_HTTPHEADER, slist);
  curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, write_body);
  curl_easy_setopt(curl_handler, CURLOPT_WRITEDATA, response);
  curl_easy_setopt(curl_handler, CURLOPT_HEADERFUNCTION, parse_header);
  curl_easy_setopt(curl_handler, CURLOPT_HEADERDATA, response);
  curl_perform_ret = curl_easy_perform(curl_handler);
  curl_easy_getinfo(curl_handler, CURLINFO_RESPONSE_CODE, &(response->status_code));
  curl_easy_cleanup(curl_handler);
  curl_slist_free_all(slist);
  free(sp);
  return response;
}

PTR_HTTP_RESPONSE https_post(char* url, PTR_HTTP_REQUEST request)
{
  int i = 0;
  char* sp = (char*) malloc(1024*sizeof(char));
  memset(sp, '\0', 1024*sizeof(char));
  CURL* curl_handler = NULL;
  CURLcode curl_perform_ret;
  struct curl_slist* slist = NULL;
  struct curl_httppost* post = NULL;
  struct curl_httppost* last = NULL;
  PTR_HTTP_RESPONSE response = alloc_http_response(0, 1024);

  curl_handler = curl_easy_init();
  curl_easy_setopt(curl_handler, CURLOPT_URL, url);
  curl_easy_setopt(curl_handler, CURLOPT_POST, 1L);

  curl_easy_setopt(curl_handler, CURLOPT_USE_SSL, CURLUSESSL_TRY);
  curl_easy_setopt(curl_handler, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(curl_handler, CURLOPT_SSL_VERIFYHOST, 0L);

  for (i=0; i<request->headers_qty; i++) {
    sprintf(sp, "%s: %s", request->headers[i].name, request->headers[i].value);
    curl_slist_append(slist, sp);
    memset(sp, '\0', 1024*sizeof(char));
  }
  curl_easy_setopt(curl_handler, CURLOPT_HTTPHEADER, slist);

  if (request->form_length == 0) {
    curl_easy_setopt(curl_handler, CURLOPT_POSTFIELDS, (void*)(request->body));
    curl_easy_setopt(curl_handler, CURLOPT_POSTFIELDSIZE, request->body_length);
  } else {
    for (i=0; i<request->form_length; i++) {
      curl_formadd(&post, &last, CURLFORM_PTRNAME, request->form[i].name, CURLFORM_PTRCONTENTS, request->form[i].value, CURLFORM_END);
    }
    curl_easy_setopt(curl_handler, CURLOPT_HTTPPOST, post);
  }
  curl_easy_setopt(curl_handler, CURLOPT_WRITEFUNCTION, write_body);
  curl_easy_setopt(curl_handler, CURLOPT_WRITEDATA, response);
  curl_easy_setopt(curl_handler, CURLOPT_HEADERFUNCTION, parse_header);
  curl_easy_setopt(curl_handler, CURLOPT_HEADERDATA, response);
  curl_perform_ret = curl_easy_perform(curl_handler);
  curl_easy_getinfo(curl_handler, CURLINFO_RESPONSE_CODE, &(response->status_code));
  curl_easy_cleanup(curl_handler);
  curl_slist_free_all(slist);
  free(sp);
  return response;
}
