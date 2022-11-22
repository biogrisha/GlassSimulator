#include "RenderModule.h"

RenderModule::RenderModule(Camera* camera, FrameViewManager* fvManager)
{
    m_camera = camera;
    m_camera->SetPosition(0, 0, 3, -90,0, 0);

    m_fvManager = fvManager;
    glGenTextures(1, &m_renderTarget);
    glBindTexture(GL_TEXTURE_2D, m_renderTarget);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glGenTextures(1, &m_depthBuffer);
    glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    fb.SetTextureAttachment(GL_COLOR_ATTACHMENT0, m_renderTarget);
    fb.SetTextureAttachment(GL_DEPTH_ATTACHMENT, m_depthBuffer);

    m_glBuf.reset(new GLBuffer(2000, 16000));
    m_frameView = m_fvManager->AddFrameView(std::make_unique<FrameViewBase>(800, 600));
    m_frameView->AddRenderUnit(std::unique_ptr<RenderUnit>(new ru_clearTg(m_frameView->GetId(), &fb)));
    m_frameView->AddRenderUnit(std::unique_ptr<RenderUnit>(new ru_drawGlass(m_frameView->GetId(), &fb,m_camera,m_glBuf.get(), m_renderTarget)));

}

RenderModule::~RenderModule()
{
    glDeleteTextures(1, &m_renderTarget);
    glDeleteTextures(1, &m_depthBuffer);
    m_fvManager->DeleteFrameView(&m_frameView);
}

void RenderModule::UpdateState()
{
    m_fvManager->RequestUpdate();
    m_fvManager->ProcessAll(m_glBuf.get());

}
