#include "sword/render.h"

#include <glad/glad.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "context.h"

typedef struct DrawTextureProgram {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
  GLuint program;
  GLint MVPLocation;
} DrawTextureProgram;

typedef struct RenderContext {
  int numDrawCall;
  SDMat3 projection;
  SDMat3 camera;
  DrawTextureProgram drawTextureProgram;
} RenderContext;

const char DRAW_TEXTURE_VERTEX_SHADER[] =
    "#version 330 core                                                      \n"
    "                                                                       \n"
    "uniform mat3 MVP;                                                      \n"
    "                                                                       \n"
    "layout (location = 0) in mat3 aTransform;                              \n"
    "layout (location = 3) in vec2 aPos;                                    \n"
    "layout (location = 4) in vec2 aTexCoord;                               \n"
    "layout (location = 5) in vec4 aColor;                                  \n"
    "out vec2 vTexCoord;                                                    \n"
    "out vec4 vColor;                                                       \n"
    "                                                                       \n"
    "void main() {                                                          \n"
    "   gl_Position = vec4(MVP * aTransform * vec3(aPos, 1), 1);            \n"
    "   vTexCoord = aTexCoord;                                              \n"
    "   vColor = aColor;                                                    \n"
    "}                                                                      \n";

const char DRAW_TEXTURE_FRAGMENT_SHADER[] =
    "#version 330 core                                                      \n"
    "                                                                       \n"
    "uniform sampler2D texture0;                                            \n"
    "                                                                       \n"
    "in vec2 vTexCoord;                                                     \n"
    "in vec4 vColor;                                                        \n"
    "                                                                       \n"
    "out vec4 fragColor;                                                    \n"
    "                                                                       \n"
    "void main() {                                                          \n"
    "   vec4 texColor = texture(texture0, vTexCoord);                       \n"
    "   // Pre-multiply alpha                                               \n"
    "   texColor = vec4(texColor.rgb * texColor.a, texColor.a);             \n"
    "                                                                       \n"
    "   fragColor = texColor * vColor;                                      \n"
    "}                                                                      \n";

typedef struct DrawTextureVertexAttrib {
  float transform0[3];
  float transform1[3];
  float transform2[3];
  float pos[2];
  float texCoord[2];
  float color[4];
} DrawTextureVertexAttrib;

static GLuint CompileGLShader(GLenum type, const char *source) {
  GLuint result = glCreateShader(type);

  glShaderSource(result, 1, &source, 0);

  glCompileShader(result);

  GLint success;
  glGetShaderiv(result, GL_COMPILE_STATUS, &success);
  if (success != GL_TRUE) {
    char buf[512];
    glGetShaderInfoLog(result, sizeof(buf), 0, buf);
    printf("Failed to compile shader: %s\n", buf);
    result = 0;
  }

  return result;
}

static GLuint CompileGLProgram(const char *vss, const char *fss) {
  GLuint result = 0;

  GLuint vs = CompileGLShader(GL_VERTEX_SHADER, vss);
  if (vs) {
    GLuint fs = CompileGLShader(GL_FRAGMENT_SHADER, fss);
    if (fs) {
      result = glCreateProgram();
      glAttachShader(result, vs);
      glAttachShader(result, fs);
      glLinkProgram(result);

      GLint success;
      glGetProgramiv(result, GL_LINK_STATUS, &success);
      if (success != GL_TRUE) {
        char buf[512];
        glGetProgramInfoLog(result, sizeof(buf), 0, buf);
        printf("Failed to link program: %s\n", buf);
        result = 0;
      }
    }
  }

  return result;
}

static void InitDrawTextureProgram(DrawTextureProgram *drawTextureProgram) {
  // Setup VAO
  glGenVertexArrays(1, &drawTextureProgram->vao);
  glGenBuffers(1, &drawTextureProgram->vbo);
  glGenBuffers(1, &drawTextureProgram->ebo);

  glBindVertexArray(drawTextureProgram->vao);
  glBindBuffer(GL_ARRAY_BUFFER, drawTextureProgram->vbo);
  glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawTextureProgram->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_STREAM_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        sizeof(DrawTextureVertexAttrib),
                        (void *)offsetof(DrawTextureVertexAttrib, transform0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                        sizeof(DrawTextureVertexAttrib),
                        (void *)offsetof(DrawTextureVertexAttrib, transform1));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                        sizeof(DrawTextureVertexAttrib),
                        (void *)offsetof(DrawTextureVertexAttrib, transform2));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
                        sizeof(DrawTextureVertexAttrib),
                        (void *)offsetof(DrawTextureVertexAttrib, pos));
  glEnableVertexAttribArray(3);

  glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE,
                        sizeof(DrawTextureVertexAttrib),
                        (void *)offsetof(DrawTextureVertexAttrib, texCoord));
  glEnableVertexAttribArray(4);

  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE,
                        sizeof(DrawTextureVertexAttrib),
                        (void *)offsetof(DrawTextureVertexAttrib, color));
  glEnableVertexAttribArray(5);

  glBindVertexArray(0);

  // Compile Program
  drawTextureProgram->program = CompileGLProgram(DRAW_TEXTURE_VERTEX_SHADER,
                                                 DRAW_TEXTURE_FRAGMENT_SHADER);
  if (!drawTextureProgram->program) {
    exit(EXIT_FAILURE);
  }
  glUseProgram(drawTextureProgram->program);
  glUniform1i(glGetUniformLocation(drawTextureProgram->program, "texture0"), 0);
  drawTextureProgram->MVPLocation =
      glGetUniformLocation(drawTextureProgram->program, "MVP");
}

extern RenderContext *CreateRenderContext(int viewportWidth, int viewportHeight,
                                          float pixelToPoint) {
  float width = viewportWidth * pixelToPoint;
  float height = viewportHeight * pixelToPoint;

  RenderContext *rc = malloc(sizeof(RenderContext));
  rc->numDrawCall = 0;
  rc->camera = SDIdentityM3();
  rc->projection = SDDotM3(
      SDMat3Translation(-1.0f, -1.0f),
      SDDotM3(
          SDMat3Scale(1.0f / width * 2.0f, 1.0f / height * 2.0f),
          SDDotM3(SDMat3Translation(0.0f, height), SDMat3Scale(1.0f, -1.0f))));

  glViewport(0, 0, viewportWidth, viewportHeight);

  glEnable(GL_BLEND);
  // Pre-multiplied alpha format
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  // Render at linear color space
  glEnable(GL_FRAMEBUFFER_SRGB);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  InitDrawTextureProgram(&rc->drawTextureProgram);

  return rc;
}

// ----------------------------------------------------------------------------
// Graphics Properties
// ----------------------------------------------------------------------------
SDAPI float SDGetCanvasWidth(void) {
  return SDGetViewportWidth() * SDGetPixelToPoint();
}

SDAPI float SDGetCanvasHeight(void) {
  return SDGetViewportHeight() * SDGetPixelToPoint();
}

SDAPI int SDGetViewportWidth(void) { return CTX->viewportWidth; }

SDAPI int SDGetViewportHeight(void) { return CTX->viewportHeight; }

SDAPI float SDGetPointToPixel(void) { return CTX->pointToPixel; }

SDAPI float SDGetPixelToPoint(void) { return CTX->pixelToPoint; }

// ----------------------------------------------------------------------------
// Image
// ----------------------------------------------------------------------------

SDAPI SDImage *SDLoadImage(const char *path) {
  SDImage *image = malloc(sizeof(SDImage));

  image->data = stbi_load(path, &image->width, &image->height, NULL, 4);
  if (image->data == NULL) {
    printf("Failed to load image %s\n", path);
    free(image);
    return NULL;
  }
  image->stride = 4 * image->width;
  image->format = SD_IMAGE_FORMAT_RGBA8;

  return image;
}

SDAPI void SDDestroyImage(SDImage **ptr) {
  SDImage *image = *ptr;
  stbi_image_free(image->data);

  *ptr = NULL;
}

// ----------------------------------------------------------------------------
// Texture
// ----------------------------------------------------------------------------

struct SDTexture {
  GLuint id;
  int actualWidth;
  int actualHeight;
  int width;
  int height;
};

static SDTexture *LoadTextureFromMemory(const void *data, int width, int height,
                                        int stride, int format) {
  SDTexture *texture = malloc(sizeof(SDTexture));
  texture->width = width;
  texture->height = height;

  glGenTextures(1, &texture->id);
  glBindTexture(GL_TEXTURE_2D, texture->id);

  texture->actualWidth = (int)SDNextPow2F((float)width);
  texture->actualHeight = height;

  int texStride = 0;
  GLint numberOfPixels = 0;
  GLint internalFormat = 0;
  GLenum glFormat = 0;

  switch (format) {
    case SD_IMAGE_FORMAT_RGBA8: {
      texStride = texture->actualWidth * 4;
      numberOfPixels = texture->actualWidth;

      internalFormat = GL_SRGB8_ALPHA8;
      glFormat = GL_RGBA;
    } break;

    case SD_IMAGE_FORMAT_A8: {
      texStride = (int)(SDCeilF(texture->actualWidth / 4.0f) * 4.0f);
      numberOfPixels = texStride;

      internalFormat = GL_R8;
      glFormat = GL_RED;

      GLint swizzleMask[] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
      glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    } break;
  }

  SDAssert(stride <= texStride);
  SDAssert(tex->actualWidth % 2 == 0);
  SDAssert(texStride % 4 == 0);

  size_t texBufLen = (size_t)texStride * texture->actualHeight;
  unsigned char *texBuf = malloc(texBufLen);
  memset(texBuf, 0, texBufLen);

  unsigned char *dstRow = texBuf;
  const unsigned char *srcRow = data;

  for (int y = 0; y < height; ++y) {
    memcpy(dstRow, srcRow, (size_t)stride);
    dstRow += texStride;
    srcRow += stride;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, numberOfPixels);
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture->actualWidth,
               texture->actualHeight, 0, glFormat, GL_UNSIGNED_BYTE, texBuf);

  free(texBuf);

  return texture;
}

SDAPI SDTexture *SDLoadTexture(const char *path) {
  SDImage *image = SDLoadImage(path);

  if (image == NULL) {
    return NULL;
  }

  SDTexture *texture = LoadTextureFromMemory(
      image->data, image->width, image->height, image->stride, image->format);

  SDDestroyImage(&image);

  return texture;
}

SDAPI void SDDestroyTexture(SDTexture **ptr) {
  SDTexture *texture = *ptr;

  glDeleteTextures(1, &texture->id);

  free(texture);

  *ptr = NULL;
}

SDAPI SDDrawTextureParams SDMakeDrawTextureParams(SDTexture *texture) {
  SDFloat pixelToPoint = SDGetPixelToPoint();
  SDDrawTextureParams params = {
      .texture = texture,
      .transform = SDIdentityM3(),
      .dstRect = SDRectMinMax(
          SDV2(0.0f, 0.0f),
          SDV2(texture->width * pixelToPoint, texture->height * pixelToPoint)),
      .srcRect = SDRectMinMax(SDV2(0.0f, 0.0f), SDV2((SDFloat)texture->width,
                                                     (SDFloat)texture->height)),
      .tintColor = SDRGBA(1.0f, 1.0f, 1.0f, 1.0f),
  };
  return params;
}

SDAPI void SDDrawTexture(const SDDrawTextureParams *params) {
  RenderContext *rc = CTX->rc;
  SDTexture *texture = params->texture;

  if (!texture) {
    return;
  }

  SDVec2 texSize =
      SDV2((SDFloat)texture->actualWidth, (SDFloat)texture->actualHeight);
  SDRect texRect = SDRectMinMax(SDHadamardDivV2(params->srcRect.min, texSize),
                                SDHadamardDivV2(params->srcRect.max, texSize));
  const float *m = (const float *)&params->transform;
  DrawTextureVertexAttrib vertices[] = {
      // top right
      m[0],
      m[1],
      m[2],
      m[3],
      m[4],
      m[5],
      m[6],
      m[7],
      m[8],
      params->dstRect.max.x,
      params->dstRect.max.y,
      texRect.max.x,
      texRect.max.y,
      params->tintColor.r,
      params->tintColor.g,
      params->tintColor.b,
      params->tintColor.a,
      // bottom right
      m[0],
      m[1],
      m[2],
      m[3],
      m[4],
      m[5],
      m[6],
      m[7],
      m[8],
      params->dstRect.max.x,
      params->dstRect.min.y,
      texRect.max.x,
      texRect.min.y,
      params->tintColor.r,
      params->tintColor.g,
      params->tintColor.b,
      params->tintColor.a,
      // bottom left
      m[0],
      m[1],
      m[2],
      m[3],
      m[4],
      m[5],
      m[6],
      m[7],
      m[8],
      params->dstRect.min.x,
      params->dstRect.min.y,
      texRect.min.x,
      texRect.min.y,
      params->tintColor.r,
      params->tintColor.g,
      params->tintColor.b,
      params->tintColor.a,
      // top left
      m[0],
      m[1],
      m[2],
      m[3],
      m[4],
      m[5],
      m[6],
      m[7],
      m[8],
      params->dstRect.min.x,
      params->dstRect.max.y,
      texRect.min.x,
      texRect.max.y,
      params->tintColor.r,
      params->tintColor.g,
      params->tintColor.b,
      params->tintColor.a,
  };

  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3,  // first triangle
      1, 2, 3,  // second triangle
  };

  glBindBuffer(GL_ARRAY_BUFFER, rc->drawTextureProgram.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc->drawTextureProgram.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STREAM_DRAW);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture->id);

  glUseProgram(rc->drawTextureProgram.program);
  SDMat3 MVP = SDDotM3(rc->projection, rc->camera);
  glUniformMatrix3fv(rc->drawTextureProgram.MVPLocation, 1, GL_FALSE,
                     (const GLfloat *)&MVP);

  glBindVertexArray(rc->drawTextureProgram.vao);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  rc->numDrawCall++;
}
