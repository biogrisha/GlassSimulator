#pragma once
#include "../RenderUnit.h"
#include <Services/BatchRendering/FrameBuffer/ImageFrameBuffer.h>
#include <Services/3DAuxiliaryFunctions/Camera.h>
#include <Services/BatchRendering/GLBuffer.h>
#include <Services/Shader/shader_s.h>
#include <GLFW/glfw3.h>
#include <Services/GlassSimulation/Splinter.h>
#include <Services/GlassSimulation/GlassLogic.h>


class ru_drawGlass : public RenderUnit
{
public:
	ru_drawGlass(uint32_t frameViewId, ImageFrameBuffer* fb, Camera* camera, GLBuffer* glBuf, GLuint texOut);
	virtual void Update() override;

private:
	void CreatePoints(std::vector<Vertex>& vertices, std::vector<int>& inds);
	ImageFrameBuffer* m_fb;
	ImageFrameBuffer m_fbForCopy;
	GLuint m_frameTexSup;
	GLuint m_frameTex;

	GLBP_wrap m_subbuf;
	std::vector<Vertex> vertices;
	std::vector<int> inds;
	std::unique_ptr<Shader> m_simpleShader;
	Camera* m_camera;
	GLBuffer* m_glBuf;

	std::unique_ptr<GlassLogic> m_glassLogic;
};