/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_NVPR_SHADER_PROGRAM_H_
#define MOZILLA_GFX_NVPR_SHADER_PROGRAM_H_

#include "2D.h"
#include "GL.h"
#include <mozilla/RefPtr.h>

namespace mozilla {
namespace gfx {
namespace nvpr {

class ShaderProgram : public RefCounted<ShaderProgram> {
public:
  ShaderProgram();
  virtual ~ShaderProgram();

  operator GLuint() { return mProgramObject; }

  void Initialize(const GLchar* aFragmentSource);

private:
  GLuint mFragmentShader;
  GLuint mProgramObject;
};

class Uniform {
public:
  friend std::ostream& operator<<(std::ostream& aOut, const Uniform& aUniform);

  void Initialize(GLuint aShaderProgram);

protected:
  Uniform(const char* aName);

  std::string mName;
  GLuint mShaderProgram;
  GLint mLocation;
};

class UniformFloat : public Uniform {
public:
  UniformFloat(const char* aName);
  void Set(GLfloat aValue);

private:
  GLfloat mValue;
};

class UniformFloatArray : public Uniform {
public:
  UniformFloatArray(const char* aName);
  void Set(GLfloat* aValues, GLsizei aCount, UniqueId aArrayId);
  void Set(GLfloat* aValues, GLsizei aCount);

private:
  UniqueId mArrayId;
};

class UniformVec2 : public Uniform {
public:
  UniformVec2(const char* aName);
  void Set(GLfloat aX, GLfloat aY);
  void Set(const Point& aXY);
  void Set(const GLfloat* aValues);

private:
  GLfloat mValues[2];
};

class UniformVec4 : public Uniform {
public:
  UniformVec4(const char* aName);
  void Set(GLfloat aX, GLfloat aY, GLfloat aZ, GLfloat aW);
  void Set(const Point& aXY, const Point& aZW);
  void Set(const GLfloat* aValues);

private:
  GLfloat mValues[4];
};

class UniformSampler : public Uniform {
public:
  UniformSampler(const char* aName, GL::TextureUnit aTextureUnit);
  void Initialize(GLuint aShaderProgram);

protected:
  GL::TextureUnit mTextureUnit;
};

}
}
}

#endif /* MOZILLA_GFX_NVPR_SHADER_PROGRAM_H_ */
