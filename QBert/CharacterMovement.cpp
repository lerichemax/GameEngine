#include "PCH.h"
#include "CharacterMovement.h"

#include "Qube.h"
#include "RendererComponent.h"
#include "ResourceManager.h"
#include "Jumper.h"

void CharacterMovement::Start()
{
	m_pRenderer = GetGameObject()->GetComponent<ECS_RendererComponent>();
	m_pJumper = GetGameObject()->GetComponent<Jumper>();

	if (m_pJumper != nullptr)
	{
		m_pJumper->OnJumpLanded.Subscribe([this](){
			JumpToCurrentQube();
			});
	}
}

void CharacterMovement::Move(ConnectionDirection direction)
{
	if (!m_CanMove)
	{
		return;
	}

	//if (!m_pCurrentQube->HasConnection(direction) && !m_pCurrentQube->HasConnectionToDisk())
	//{
	//	JumpToDeath(direction);
	//	SwitchState(new FallingState(this, m_pJumper));
	//	SoundServiceLocator::GetService().Play(m_FallSoundID, 50);
	//	return;
	//}

	//SetDirectionTextures(direction);
	//
	m_CurentDirection = direction;
	if (m_pCurrentQube->HasConnection(m_CurentDirection))
	{
		SetJumpTexture();
		OnMoveStarted.Notify();
		m_pCurrentQube->CharacterJumpOut();
		m_pCurrentQube = m_pCurrentQube->GetConnection(m_CurentDirection);
		m_pJumper->Jump(GetGameObject()->GetTransform()->GetPosition(), m_pCurrentQube->GetCharacterPos());
		m_CanMove = false;
		//SwitchState(new JumpingState(this, m_pJumper));
	}
	else
	{
		float dist{};
		if (m_CurentDirection == ConnectionDirection::downLeft || m_CurentDirection == ConnectionDirection::upLeft)
		{
			dist = -25.f;
		}
		else if (m_CurentDirection == ConnectionDirection::downRight || m_CurentDirection == ConnectionDirection::upRight)
		{
			dist = 25.f;
		}

		m_pCurrentQube->CharacterJumpOut();
		m_pJumper->JumpToDeath(GetGameObject()->GetTransform()->GetPosition(), dist);
	}
	//else if(m_pCurrentQube->HasConnectionToDisk())
	//{
	//	SoundServiceLocator::GetService().Play(m_JumpSoundID, 50);
	//	//m_pGameObject->GetComponent<BoxCollider>()->SetEnable(false);
	//	m_pCurrentQube->GetConnectedDisk()->ReceivePlayer(this);
	//	m_pCurrentQube->CharacterJumpOut();
	//	m_pCurrentQube = nullptr;
	//	m_bCanMove = false;
	//}
}

void CharacterMovement::SetCurrentQube(std::shared_ptr<Qube> pQube)
{
	if (pQube != nullptr)
	{
		m_pCurrentQube = pQube;
	}
	
	MoveToCurrentQube();
}

std::shared_ptr<Qube> CharacterMovement::GetCurrentQube() const
{
	return m_pCurrentQube;
}


void CharacterMovement::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterMovement).name());

	writer.WriteString("textIdleDownRight", m_TextureIdleDownRight);
	writer.WriteString("textIdleDownLeft", m_TextureIdleDownLeft);
	writer.WriteString("textIdleUpRight", m_TextureIdleUpRight);
	writer.WriteString("textIdleUpLeft", m_TextureIdleUpLeft);

	writer.WriteString("textJumpDownRight", m_TextureJumpDownRight);
	writer.WriteString("textJumpDownLeft", m_TextureJumpDownLeft);
	writer.WriteString("textJumpUpRight", m_TextureJumpUpRight);
	writer.WriteString("textJumpUpLeft", m_TextureJumpUpLeft);

	BehaviourComponent::Serialize(writer);
}

void CharacterMovement::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	BehaviourComponent::Deserialize(reader, context);

	reader->ReadString("textIdleDownRight", m_TextureIdleDownRight);
	reader->ReadString("textIdleDownLeft", m_TextureIdleDownLeft);
	reader->ReadString("textIdleUpRight", m_TextureIdleUpRight);
	reader->ReadString("textIdleUpLeft", m_TextureIdleUpLeft);

	reader->ReadString("textJumpDownRight", m_TextureJumpDownRight);
	reader->ReadString("textJumpDownLeft", m_TextureJumpDownLeft);
	reader->ReadString("textJumpUpRight", m_TextureJumpUpRight);
	reader->ReadString("textJumpUpLeft", m_TextureJumpUpLeft);
}

void CharacterMovement::SetTextureIdleNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft)
{
	m_TextureIdleDownRight = downRight;
	m_TextureIdleDownLeft = downLeft;
	m_TextureIdleUpRight = upRight;
	m_TextureIdleUpLeft = upLeft;
}

void CharacterMovement::SetTextureJumpNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft)
{
	m_TextureJumpDownRight = downRight;
	m_TextureJumpDownLeft = downLeft;
	m_TextureJumpUpRight = upRight;
	m_TextureJumpUpLeft = upLeft;
}

void CharacterMovement::JumpToCurrentQube()
{
	if (m_pCurrentQube == nullptr)
	{
		return;
	}

	MoveToCurrentQube();

	//temp ?
	m_pCurrentQube->HandleQBertLanding();
	m_CanMove = true;
}

void CharacterMovement::MoveToCurrentQube()
{
	if (m_pCurrentQube == nullptr)
	{
		return;
	}

	//if (m_pCurrentQube->HasCharacter() && m_pCurrentQube->GetCharacter() != this)
	//{
	//	MeetCharacter(m_pCurrentQube->GetCharacter());
	//}

	if (GetGameObject()->IsActive())
	{
		//m_pCurrentQube->CharacterJumpIn(this);
		GetGameObject()->GetTransform()->Translate(m_pCurrentQube->GetCharacterPos());
	}

	SetIdleTexture();
	m_CanMove = true;
}

void CharacterMovement::SetIdleTexture()
{
	if (m_pRenderer == nullptr)
	{
		return;
	}

	switch (m_CurentDirection)
	{
	case ConnectionDirection::downLeft:
		m_pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(m_TextureIdleDownLeft);
		break;
	case ConnectionDirection::downRight:
		m_pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(m_TextureIdleDownRight);
		break;
	case ConnectionDirection::upLeft:
		m_pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(m_TextureIdleUpLeft);
		break;
	case ConnectionDirection::upRight:
		m_pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(m_TextureIdleUpRight);
		break;
	}
}

void CharacterMovement::SetJumpTexture()
{
	if (m_pRenderer == nullptr)
	{
		return;
	}

	switch (m_CurentDirection)
	{
	case ConnectionDirection::downLeft:
		m_pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(m_TextureJumpDownLeft);
		break;
	case ConnectionDirection::downRight:
		m_pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(m_TextureJumpDownRight);
		break;
	case ConnectionDirection::upLeft:
		m_pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(m_TextureJumpUpLeft);
		break;
	case ConnectionDirection::upRight:
		m_pRenderer->m_pTexture = ResourceManager::GetInstance().GetTexture(m_TextureJumpUpRight);
		break;
	}
}