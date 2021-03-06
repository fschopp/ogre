/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __OgreGLRenderSystemCommon_H__
#define __OgreGLRenderSystemCommon_H__

#include "OgreGLSupportPrerequisites.h"
#include "OgreRenderSystem.h"
#include "OgreRenderWindow.h"

namespace Ogre {
    class GLContext;

    class _OgreGLExport GLRenderSystemCommon : public RenderSystem
    {
    protected:
        /* The main GL context - main thread only */
        GLContext* mMainContext;

        /* The current GL context  - main thread only */
        GLContext* mCurrentContext;
    public:
        virtual ~GLRenderSystemCommon() {}

        /** @copydoc RenderTarget::copyContentsToMemory */
        virtual void _copyContentsToMemory(Viewport* vp, const Box& src, const PixelBox& dst,
                                           RenderWindow::FrameBuffer buffer) = 0;

        /** Returns the main context */
        GLContext* _getMainContext() {
            return mMainContext;
        }

        /** Unregister a render target->context mapping. If the context of target
            is the current context, change the context to the main context so it
            can be destroyed safely.

            @note This is automatically called by the destructor of
            GLContext.
        */
        virtual void _unregisterContext(GLContext *context) = 0;

        Real getHorizontalTexelOffset(void) { return 0.0; }               // No offset in GL
        Real getVerticalTexelOffset(void) { return 0.0; }                 // No offset in GL
        Real getMinimumDepthInputValue(void) { return -1.0f; }            // Range [-1.0f, 1.0f]
        Real getMaximumDepthInputValue(void) { return 1.0f; }             // Range [-1.0f, 1.0f]

        VertexElementType getColourVertexElementType(void) const {
            return VET_COLOUR_ABGR;
        }

        void reinitialise(void)
        {
            this->shutdown();
            this->_initialise(true);
        }

        void _convertProjectionMatrix(const Matrix4& matrix, Matrix4& dest, bool)
        {
            // no conversion request for OpenGL
            dest = matrix;
        }

        void _makeProjectionMatrix(const Radian& fovy, Real aspect, Real nearPlane, Real farPlane,
                                   Matrix4& dest, bool forGpuProgram = false);

        void _makeProjectionMatrix(Real left, Real right, Real bottom, Real top,
                                   Real nearPlane, Real farPlane, Matrix4& dest, bool forGpuProgram = false);

        void _makeOrthoMatrix(const Radian& fovy, Real aspect, Real nearPlane, Real farPlane,
                              Matrix4& dest, bool forGpuProgram = false);

        void _applyObliqueDepthProjection(Matrix4& matrix, const Plane& plane,
                                          bool forGpuProgram);

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID || OGRE_PLATFORM == OGRE_PLATFORM_EMSCRIPTEN
        static void _destroyInternalResources(RenderWindow* pRenderWnd);
        static void _createInternalResources(RenderWindow* pRenderWnd, void* nativeWindow, void* config = NULL);

        virtual void resetRenderer(RenderWindow* pRenderWnd) = 0;
        virtual void notifyOnContextLost() = 0;
#endif
    };
}

#endif
