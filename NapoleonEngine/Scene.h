#pragma once
#include "SceneManager.h"
#include "ColliderComponent.h"
#include "Entity.h"

#include <memory>

enum class Layer;
class SceneRenderer;
class RendererComponent;
class GameObject;
class CameraComponent;
class CameraSystem;
class TransformSystem;
class Coordinator;
class LayeredRendererSystem;
class Scene
{
	friend class SceneManager;
public:
	explicit Scene(const std::string& name);
	virtual ~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;
		
	void AddObject(GameObject* object); //deprecated
	GameObject* CreateGameObject();
		
	void Render() const;
	bool IsActive() const { return m_bIsActive; }
	void AddToGroup(RendererComponent* pRenderer, Layer layer) const;
	void RemoveFromGroup(RendererComponent* pRenderer, Layer layer) const;
	void SetCameraActive(CameraComponent* pCamera) { m_pActiveCamera = pCamera; }
	std::string GetName()const { return m_Name; }
	
protected:
	virtual void CustomOnActivate(){}
	void SetActiveCamera(Entity entity);

	std::shared_ptr<Coordinator> m_pRegistry;
	
private:
	friend void ColliderComponent::Initialize();
	friend ColliderComponent::~ColliderComponent();
	friend class SceneManager;
		
	void AddCollider(ColliderComponent* pCollider);
	void RemoveCollider(ColliderComponent* pCollider);
		
	std::string m_Name;
	std::vector<GameObject*> m_pObjects;
	std::vector<ColliderComponent*> m_pColliders;
		
	SceneRenderer* m_pSceneRenderer; //deprecated
	std::shared_ptr<LayeredRendererSystem> m_pECS_SceneRenderer;
	std::shared_ptr<TransformSystem> m_pTransformSystem;
	std::shared_ptr<CameraSystem> m_pCamera;
	CameraComponent* m_pActiveCamera;
	

		
	bool m_bIsActive;
	bool m_bIsInitialized;

	virtual void Initialize() {};
	void Update();
	virtual void DeclareInput() {};
		
	void OnActivate();
	void Refresh();
	void CleanUpScene();
	void CheckCollidersCollision();
		
};
