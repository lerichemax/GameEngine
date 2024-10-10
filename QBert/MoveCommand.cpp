#include "PCH.h"
#include "MoveCommand.h"

#include "CharacterState.h"
#include "GameObject.h"
#include "QubeComponent.h"

MoveCommand::MoveCommand(ConnectionDirection dir)
	:m_MoveDirection{ dir }
{}

void MoveCommand::Execute(GameObject const& gObject)
{
	if (!gObject.IsActive()/* || m_pCharacter->GetState()->GetType() != CharacterStateType::onQube*/)
	{
		return;
	}

	//CharacterMovement* const pMover = gObject->GetComponent<CharacterMovement>();

	//if (pMover != nullptr)
	//{
	//	pMover->Move(m_MoveDirection);
	//}
}