/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_NVPR_WGLDEFS_H_
#define MOZILLA_GFX_NVPR_WGLDEFS_H_

#include "GLDefs.h"
#include "Windows.h"

// WGL_ARB_extensions_string
typedef const char * (WINAPI * WGLGetExtensionsStringARB) (HDC hdc);


// WGL_NV_copy_image
typedef BOOL (WINAPI * WGLCopyImageSubDataNV) (HGLRC hSrcRC, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, HGLRC hDstRC, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);


// WGL_NV_copy_image
#define WGL_ACCESS_READ_ONLY_NV        0x00000000
#define WGL_ACCESS_READ_WRITE_NV       0x00000001
#define WGL_ACCESS_WRITE_DISCARD_NV    0x00000002

typedef BOOL (WINAPI * WGLDXSetResourceShareHandleNV) (void *dxObject, HANDLE shareHandle);
typedef HANDLE (WINAPI * WGLDXOpenDeviceNV) (void *dxDevice);
typedef BOOL (WINAPI * WGLDXCloseDeviceNV) (HANDLE hDevice);
typedef HANDLE (WINAPI * WGLDXRegisterObjectNV) (HANDLE hDevice, void *dxObject, GLuint name, GLenum type, GLenum access);
typedef BOOL (WINAPI * WGLDXUnregisterObjectNV) (HANDLE hDevice, HANDLE hObject);
typedef BOOL (WINAPI * WGLDXObjectAccessNV) (HANDLE hObject, GLenum access);
typedef BOOL (WINAPI * WGLDXLockObjectsNV) (HANDLE hDevice, GLint count, HANDLE *hObjects);
typedef BOOL (WINAPI * WGLDXUnlockObjectsNV) (HANDLE hDevice, GLint count, HANDLE *hObjects);


#endif /* MOZILLA_GFX_NVPR_WGLDEFS_H_ */
