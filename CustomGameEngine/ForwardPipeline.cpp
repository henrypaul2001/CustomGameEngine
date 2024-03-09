#include "ForwardPipeline.h"
#include "ResourceManager.h"
#include "ComponentLight.h"
#include "LightManager.h"

#include "SystemRender.h"'
#include "SystemShadowMapping.h"
namespace Engine {
	ForwardPipeline::ForwardPipeline()
	{

	}

	ForwardPipeline::~ForwardPipeline()
	{

	}

	void ForwardPipeline::Run(std::vector<System*> renderSystems, std::vector<Entity*> entities)
	{
		RenderPipeline::Run(renderSystems, entities);

		// shadow map steps
		if (shadowmapSystem != nullptr) {
			RunShadowMapSteps();
		}

		// render scene to textured framebuffer
		if (renderSystem != nullptr) {

			SceneRenderStep();

			ScreenTextureStep();
		}
	}

	void ForwardPipeline::SceneRenderStep()
	{
		glViewport(0, 0, screenWidth, screenHeight);

		// Render to textured framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, *renderInstance->GetTexturedFBO());

		//glCullFace(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		for (Entity* e : entities) {
			renderSystem->OnAction(e);
		}
		renderSystem->AfterAction();
	}

	void ForwardPipeline::ScreenTextureStep()
	{
		// render final scene texture on screen quad
		glViewport(0, 0, screenWidth, screenHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		Shader* screenQuadShader = ResourceManager::GetInstance()->ScreenQuadShader();
		screenQuadShader->Use();

		screenQuadShader->setUInt("postProcess", renderSystem->GetPostProcess());

		if (renderSystem->GetPostProcess() == CUSTOM_KERNEL) {
			screenQuadShader->setFloat("customKernel[0]", renderSystem->PostProcessKernel[0]);
			screenQuadShader->setFloat("customKernel[1]", renderSystem->PostProcessKernel[1]);
			screenQuadShader->setFloat("customKernel[2]", renderSystem->PostProcessKernel[2]);
			screenQuadShader->setFloat("customKernel[3]", renderSystem->PostProcessKernel[3]);
			screenQuadShader->setFloat("customKernel[4]", renderSystem->PostProcessKernel[4]);
			screenQuadShader->setFloat("customKernel[5]", renderSystem->PostProcessKernel[5]);
			screenQuadShader->setFloat("customKernel[6]", renderSystem->PostProcessKernel[6]);
			screenQuadShader->setFloat("customKernel[7]", renderSystem->PostProcessKernel[7]);
			screenQuadShader->setFloat("customKernel[8]", renderSystem->PostProcessKernel[8]);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *renderInstance->GetScreenTexture());
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		ResourceManager::GetInstance()->DefaultPlane()->Draw(*screenQuadShader);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}
}