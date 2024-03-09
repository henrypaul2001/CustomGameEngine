#include "RenderPipeline.h"
#include "SystemRender.h"'
#include "SystemShadowMapping.h"
#include "ResourceManager.h"
#include "ComponentLight.h"
#include "LightManager.h"
namespace Engine {
	RenderPipeline::RenderPipeline()
	{

	}

	RenderPipeline::~RenderPipeline()
	{

	}

	void RenderPipeline::Run(std::vector<System*> renderSystems, std::vector<Entity*> entities)
	{
		this->entities = entities;

		shadowmapSystem = nullptr;
		renderSystem = nullptr;
		renderInstance = RenderManager::GetInstance();

		shadowWidth = renderInstance->ShadowWidth();
		shadowHeight = renderInstance->ShadowHeight();

		screenWidth = renderInstance->ScreenWidth();
		screenHeight = renderInstance->ScreenHeight();

		for (System* s : renderSystems) {
			if (s->Name() == SYSTEM_RENDER) {
				renderSystem = dynamic_cast<SystemRender*>(s);
			}
			else if (s->Name() == SYSTEM_SHADOWMAP) {
				shadowmapSystem = dynamic_cast<SystemShadowMapping*>(s);
			}
		}
	}

	void RenderPipeline::DirLightShadowStep()
	{
		// Directional light
		renderInstance->BindShadowMapTextureToFramebuffer(-1, MAP_2D); // bind the dir light shadowmap to framebuffer
		unsigned int shadowWidth = renderInstance->ShadowWidth(); // in future, these will be stored in the light component
		unsigned int shadowHeight = renderInstance->ShadowHeight(); // <--/

		ComponentLight* dirLight = dynamic_cast<ComponentLight*>(LightManager::GetInstance()->GetDirectionalLightEntity()->GetComponent(COMPONENT_LIGHT));
		glm::vec3 lightPos = -dirLight->Direction * dirLight->DirectionalLightDistance; // negative of the directional light's direction
		float orthoSize = dirLight->ShadowProjectionSize;
		float near = dirLight->Near;
		float far = dirLight->Far;
		glm::mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, near, far);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		depthShader->Use();
		depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, *depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		shadowmapSystem->SetDepthMapType(MAP_2D);
		for (Entity* e : entities) {
			shadowmapSystem->OnAction(e);
		}
		shadowmapSystem->AfterAction();
	}

	void RenderPipeline::ActiveLightsShadowStep()
	{
		// Spot and point lights
		std::vector<glm::mat4> shadowTransforms;
		float aspect = (float)shadowWidth / (float)shadowHeight;
		std::vector<Entity*> lightEntities = LightManager::GetInstance()->GetLightEntities();
		for (int i = 0; i < lightEntities.size() && i < 8; i++) {
			ComponentLight* lightComponent = dynamic_cast<ComponentLight*>(lightEntities[i]->GetComponent(COMPONENT_LIGHT));
			ComponentTransform* transformComponent = dynamic_cast<ComponentTransform*>(lightEntities[i]->GetComponent(COMPONENT_TRANSFORM));

			if (lightComponent->GetLightType() == SPOT) {
				renderInstance->BindShadowMapTextureToFramebuffer(i, MAP_2D);

				glm::vec3 lightPos = transformComponent->GetWorldPosition();
				glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), aspect, lightComponent->Near, lightComponent->Far);
				glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightComponent->Direction, glm::vec3(0.0f, 1.0f, 0.0f));
				glm::mat4 lightSpaceMatrix = lightProjection * lightView;

				depthShader->Use();
				depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

				glViewport(0, 0, shadowWidth, shadowHeight);
				glBindFramebuffer(GL_FRAMEBUFFER, *depthMapFBO);
				glClear(GL_DEPTH_BUFFER_BIT);

				//glEnable(GL_CULL_FACE);
				//glCullFace(GL_FRONT);
				shadowmapSystem->SetDepthMapType(MAP_2D);
				for (Entity* e : entities) {
					shadowmapSystem->OnAction(e);
				}
				shadowmapSystem->AfterAction();
			}
			else if (lightComponent->GetLightType() == POINT) {
				glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), aspect, lightComponent->Near, lightComponent->Far);
				glm::vec3 lightPos = transformComponent->GetWorldPosition();

				shadowTransforms.clear();
				shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
				shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
				shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
				shadowTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

				cubeDepthShader->Use();
				for (unsigned int i = 0; i < 6; ++i) {
					cubeDepthShader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
					cubeDepthShader->setFloat("far_plane", lightComponent->Far);
					cubeDepthShader->setVec3("lightPos", lightPos);
				}

				renderInstance->BindShadowMapTextureToFramebuffer(i, MAP_CUBE);
				glViewport(0, 0, shadowWidth, shadowHeight);
				glBindFramebuffer(GL_FRAMEBUFFER, *cubeDepthMapFBO);
				glClear(GL_DEPTH_BUFFER_BIT);

				shadowmapSystem->SetDepthMapType(MAP_CUBE);
				for (Entity* e : entities) {
					shadowmapSystem->OnAction(e);
				}
				shadowmapSystem->AfterAction();
			}
		}
	}

	void RenderPipeline::RunShadowMapSteps()
	{
		depthShader = ResourceManager::GetInstance()->ShadowMapShader();
		cubeDepthShader = ResourceManager::GetInstance()->CubeShadowMapShader();
		depthMapFBO = renderInstance->GetFlatDepthFBO();
		cubeDepthMapFBO = renderInstance->GetCubeDepthFBO();

		DirLightShadowStep();

		ActiveLightsShadowStep();
	}
}