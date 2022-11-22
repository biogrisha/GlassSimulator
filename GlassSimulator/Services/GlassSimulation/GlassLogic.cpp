#include "GlassLogic.h"

GlassLogic::GlassLogic(Camera* camera, GLBuffer* glBuf)
{
    m_search.glBuf = glBuf;
    m_search.subbuf = m_search.glBuf->NewSubBuffer(1000, 8000);

    m_search.camera = camera;
    std::vector<Shader::ShaderData> shData;
    shData.push_back(Shader::ShaderData(Shader::Type::VERTEX, "Shaders\\search.vs"));
    shData.push_back(Shader::ShaderData(Shader::Type::GEOMETRY, "Shaders\\search.gs"));
    shData.push_back(Shader::ShaderData(Shader::Type::FRAGMENT, "Shaders\\search.fs"));

    m_search.shader.reset(new Shader(shData));

    glGenTextures(1, &m_search.rTarget);
    glBindTexture(GL_TEXTURE_2D, m_search.rTarget);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, 800, 600, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &m_search.depthBuf);
    glBindTexture(GL_TEXTURE_2D, m_search.depthBuf);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    m_search.fb.SetTextureAttachment(GL_COLOR_ATTACHMENT0, m_search.rTarget);
    m_search.fb.SetTextureAttachment(GL_DEPTH_ATTACHMENT, m_search.depthBuf);
    glBindTexture(GL_TEXTURE_2D, 0);


}

void GlassLogic::MakeFirstBreak()
{
    if (m_states.glassBlockCreated)
    {
        int count = 100;
        float squaredCount = pow(count, 1);

        float posX = 0.5;
        float posY = 0.5;

        int coefx[4] = { -1,1,1,-1 };
        int coefy[4] = { -1,1,-1,1 };
        int coefCount = 0;

        int closenessInd = count;
        m_blockVertices.resize(4);
        for (float i = 0; i < count; i++)
        {
            if (int(i) % 4 == 0)
            {
                closenessInd--;
            }
            float ix = pow(closenessInd, 1) * coefx[coefCount];
            float iy = pow(closenessInd, 1) * coefy[coefCount];

            //float x = ((float(rand() % 100)) /200)*(ix/ squaredCount)+posX;
            //float y = ((float(rand() % 100)) /200)*(iy/ squaredCount)+posY;

            float x = ((float(rand() % 1000)) / 1000);
            float y = ((float(rand() % 1000)) / 1000);
            m_blockVertices.push_back({ glm::vec4(x,y,0,0),glm::vec4((float(rand() % 100)) / 100,(float(rand() % 100)) / 100,(float(rand() % 100)) / 100,1) });


            coefCount++;
            if (coefCount > 3)
            {
                coefCount = 0;
            }
        }

        m_firstSplinters.reset(new Splinters(m_blockVertices));


        m_states.pointsMustUpdate = true;

        m_states.firstTriangulation = true;
    }
}

void GlassLogic::MakeBreak(uint32_t x, uint32_t y)
{
    if (m_states.firstTriangulation)
    {

        Splinter& splint = m_firstSplinters->GetSplinters()[y];

        std::vector<Vertex> verts;
        verts.reserve(23);
        verts.push_back(m_firstSplinters->GetPoints()[splint.m_triangles[0].a]);
        verts.push_back(m_firstSplinters->GetPoints()[splint.m_triangles[0].b]);
        verts.push_back(m_firstSplinters->GetPoints()[splint.m_triangles[0].c]);


        PointsInsideTriangle pointsGen;
        pointsGen.GeneratePoints(verts, verts[0].pos, verts[1].pos, verts[2].pos,20);

        glm::vec4 ab = verts[1].pos - verts[0].pos;
        glm::vec4 ac = verts[2].pos - verts[0].pos;
        glm::vec4 a = verts[0].pos;

        glm::mat2x2 trans(ab.x, ab.y, ac.x, ac.y);
        auto invTrans = glm::inverse(trans);

        for (auto& vert : verts)
        {
            auto vecTemp = vert.pos - a;

            vert.pos.x = (vecTemp.x * invTrans[0].x + vecTemp.y * invTrans[1].x);
            vert.pos.y = (vecTemp.x * invTrans[0].y + vecTemp.y * invTrans[1].y);
        }
        m_secondSplinters.push_back(Splinters(verts));

        auto& verticesSplint = m_secondSplinters.back().GetPoints();
        for (auto& vert : verticesSplint)
        {
            auto vecTemp = vert.pos;
            //vert.pos.x += 1;
            //vert.pos.y += 1;
            vert.pos.x = (vecTemp.x * trans[0].x + vecTemp.y * trans[1].x)+a.x;
            vert.pos.y = (vecTemp.x * trans[0].y + vecTemp.y * trans[1].y)+a.y;
        }
        m_states.pointsMustUpdate = true;
        std::iter_swap(m_firstSplinters->GetSplinters().begin() + y, m_firstSplinters->GetSplinters().end() - 1);
        m_firstSplinters->GetSplinters().pop_back();
    }
}

void GlassLogic::CreateGlassBlock()
{
    m_blockVertices.push_back({ glm::vec4(0,0,0,0),glm::vec4(1,0,0,1) });
    m_blockVertices.push_back({ glm::vec4(0,1,0,0),glm::vec4(1,0,0,1) });
    m_blockVertices.push_back({ glm::vec4(1,0,0,0),glm::vec4(1,0,0,1) });
    m_blockVertices.push_back({ glm::vec4(1,1,0,0),glm::vec4(1,0,0,1) });

    m_blockVertices.push_back({ glm::vec4(0,0,-0.1,0),glm::vec4(1,0,0,1) });
    m_blockVertices.push_back({ glm::vec4(0,1,-0.1,0),glm::vec4(1,0,0,1) });
    m_blockVertices.push_back({ glm::vec4(1,0,-0.1,0),glm::vec4(1,0,0,1) });
    m_blockVertices.push_back({ glm::vec4(1,1,-0.1,0),glm::vec4(1,0,0,1) });

    m_blockInds = {
        0,1,2,
        1,2,3,
        4,5,6,
        5,6,7,
        0,1,4,
        1,4,5,
        1,3,5,
        3,5,7,
        2,3,6,
        3,6,7,
        0,2,4,
        2,4,6
    };

    m_states.glassBlockCreated = true;
}

std::vector<Vertex>& GlassLogic::GetVertices()
{
    UpdateData();
    return m_blockVertices;
}

std::vector<int>& GlassLogic::GetIndices()
{
    UpdateData();
    return m_blockInds;
}

void GlassLogic::UpdateData()
{
    if (m_states.firstTriangulation && m_states.pointsMustUpdate)
    {
        m_blockVertices = m_firstSplinters->GetPoints();
        m_blockInds.clear();

        {
            auto& splintersVec = m_firstSplinters->GetSplinters();

            for (auto& splint : splintersVec)
            {
                for (auto& trig : splint.m_triangles)
                {
                    m_blockInds.push_back(trig.a);
                    m_blockInds.push_back(trig.b);
                    m_blockInds.push_back(trig.c);
                }
            }
        }

        for (auto& splints : m_secondSplinters)
        {
            int vertSize = m_blockVertices.size();
            auto& splintersVec = splints.GetSplinters();

            for (auto& splint : splintersVec)
            {
                for (auto& trig : splint.m_triangles)
                {
                    m_blockInds.push_back(trig.a+vertSize);
                    m_blockInds.push_back(trig.b+vertSize);
                    m_blockInds.push_back(trig.c+vertSize);
                }
            }
            m_blockVertices.insert(m_blockVertices.end(), splints.GetPoints().begin(), splints.GetPoints().end());
        }
        m_states.pointsMustUpdate = false;
    }
}

int GlassLogic::FindSplinter(uint32_t triangleId)
{
    int res = -1;
    if (triangleId < m_firstSplinters->GetSplinters().size() * 8)
    {
        res = triangleId / 8;
    }
    return res;
}

uint32_t GlassLogic::FindTriangeId(uint32_t x, uint32_t y)
{
    UpdateData();
    m_search.subbuf->UpdateVertices(0, m_blockVertices);
    m_search.subbuf->UpdateIndices(0, m_blockInds);
    m_search.subbuf->SetIndexCount(m_blockInds.size());
    m_search.shader->use();
    glm::mat4 projection = glm::perspective(glm::radians(m_search.camera->Zoom), (float)800 / (float)600, 0.5f, 50.0f);
    glm::mat4 view = m_search.camera->GetViewMatrix();

    m_search.shader->setMat4("projection", projection);
    m_search.shader->setMat4("view", view);

    m_search.fb.Bind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, m_search.subbuf->GetIndexCount(), GL_UNSIGNED_INT, (void*)(m_search.subbuf->GetIndexOffset() * sizeof(GLuint)));

    unsigned char pixels[1 * 1 * 12] = { 0 };
    static unsigned int val = 0; 
    glReadPixels(400, 300, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT,pixels);
    m_search.fb.Unbind();

    return *((unsigned int*)pixels);
}

