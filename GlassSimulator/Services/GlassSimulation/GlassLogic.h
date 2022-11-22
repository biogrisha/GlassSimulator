#pragma once
#include <memory>
#include <Services/BatchRendering/FrameBuffer/ImageFrameBuffer.h>
#include <Services/GlassSimulation/Splinter.h>
#include <Services/3DAuxiliaryFunctions/PointsGenerators/PointsInsideTriangle.h>
#include <Services/3DAuxiliaryFunctions/Camera.h>
#include <Services/Shader/shader_s.h>
#include <Services/BatchRendering/GLBuffer.h>
class GlassLogic
{
public:
	GlassLogic(Camera* camera, GLBuffer* glBuf);
	void MakeFirstBreak();
	void MakeBreak(uint32_t x, uint32_t y);
	void CreateGlassBlock();
	std::vector<Vertex>& GetVertices();
	std::vector<int>& GetIndices();
private:
	int FindSplinter(uint32_t triangleId);
	uint32_t FindTriangeId(uint32_t x, uint32_t y);
	void UpdateData();
	struct
	{
		bool glassBlockCreated = false;
		bool firstTriangulation = false;
		bool pointsMustUpdate = false;
	}m_states;

	struct
	{
		ImageFrameBuffer fb;
		GLuint rTarget;
		GLuint depthBuf;

		std::unique_ptr<Shader> shader;
		Camera* camera;
		GLBuffer* glBuf;
		GLBP_wrap subbuf;
	}m_search;

	int secondBreaks = 0;
	std::vector<Vertex> m_blockVertices;
	std::vector<int> m_blockInds;
	std::unique_ptr<Splinters> m_firstSplinters;
	std::vector<Splinters> m_secondSplinters;
	
};