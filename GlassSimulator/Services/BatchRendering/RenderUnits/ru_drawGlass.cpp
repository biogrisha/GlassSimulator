#include "ru_drawGlass.h"

ru_drawGlass::ru_drawGlass(uint32_t frameViewId, ImageFrameBuffer* fb, Camera* camera, GLBuffer* glBuf, GLuint texOut) : RenderUnit(frameViewId)
{
	m_fb = fb;
    m_glBuf = glBuf;
    m_subbuf = m_glBuf->NewSubBuffer(1000, 8000);
    m_simpleShader.reset(new Shader("Shaders\\vshad.vs", "Shaders\\fshad.fs"));
    m_camera = camera;
    m_frameTex = texOut;
    m_glassLogic.reset(new GlassLogic(m_camera, m_glBuf));

    glGenTextures(1, &m_frameTexSup);
    glBindTexture(GL_TEXTURE_2D, m_frameTexSup);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_fbForCopy.SetTextureAttachment(GL_COLOR_ATTACHMENT0, m_frameTex, GL_READ_FRAMEBUFFER);
    m_fbForCopy.SetTextureAttachment(GL_COLOR_ATTACHMENT1, m_frameTexSup, GL_DRAW_FRAMEBUFFER);


    CreatePoints(vertices, inds);
    
   
    m_subbuf->UpdateVertices(0, vertices);
    m_subbuf->UpdateIndices(0, inds);
    m_subbuf->SetIndexCount(inds.size());


   

}

void ru_drawGlass::Update()
{
    

    m_simpleShader->use();
    glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)800 / (float)600, 0.5f, 50.0f);
    glm::mat4 view = m_camera->GetViewMatrix();

    m_simpleShader->setMat4("projection", projection);
    m_simpleShader->setMat4("view", view);
    m_simpleShader->setVec2("resolution", glm::vec2(800, 600));
    m_simpleShader->setInt("texture1", 0);
    m_simpleShader->setInt("texture2", 1);
    m_simpleShader->setVec3("viewPos", m_camera->Position);





    //========================================

    m_fb->Bind();
    m_simpleShader->setInt("mod", 3);
    
    glDrawElements(GL_TRIANGLES, m_subbuf->GetIndexCount(), GL_UNSIGNED_INT, (void*)(m_subbuf->GetIndexOffset() * sizeof(GLuint)));
    
    m_fb->Unbind();
    
    m_simpleShader->setInt("mod", 1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_frameTexSup);
   // ==================================
    
    
    m_fbForCopy.Bind();
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
    m_fbForCopy.Unbind();
    
    m_fb->Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, m_subbuf->GetIndexCount(), GL_UNSIGNED_INT, (void*)(m_subbuf->GetIndexOffset() * sizeof(GLuint)));
    m_fb->Unbind();
    m_fbForCopy.Bind();
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
    m_fbForCopy.Unbind();
    
    m_simpleShader->setInt("mod", 2);
    
    m_fbForCopy.Bind();
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);
    m_fbForCopy.Unbind();
    m_fb->Bind();
    glDrawElements(GL_TRIANGLES, m_subbuf->GetIndexCount(), GL_UNSIGNED_INT, (void*)(m_subbuf->GetIndexOffset() * sizeof(GLuint)));
    
    m_fb->Unbind();
    
    
    //==================================
    glBindTexture(GL_TEXTURE_2D, 0);


    
    //glPointSize(5);
    //glEnable(GL_DEPTH);
    //glDepthFunc(GL_ALWAYS);
    //m_fb->Bind();
    //m_simpleShader->setInt("mod", 0);
    ////glDrawElements(GL_POINTS, vertices.size(), GL_UNSIGNED_INT, (void*)(m_subbuf->GetIndexOffset() * sizeof(GLuint)));
    //glDrawElements(GL_POINTS, vertices.size(), GL_UNSIGNED_INT, (void*)((m_subbuf->GetIndexOffset()) * sizeof(GLuint)));
    //glDrawElements(GL_TRIANGLES, m_subbuf->GetIndexCount()- vertices.size(), GL_UNSIGNED_INT, (void*)((m_subbuf->GetIndexOffset()+ vertices.size()) * sizeof(GLuint)));
    //
    //m_fb->Unbind();



}

void ru_drawGlass::CreatePoints(std::vector<Vertex>& vertices, std::vector<int>& inds)
{

    m_glassLogic->CreateGlassBlock();
    m_glassLogic->MakeFirstBreak();
    //m_glassLogic->MakeBreak(1,3);
    vertices = m_glassLogic->GetVertices();
    inds = m_glassLogic->GetIndices();

    
}

