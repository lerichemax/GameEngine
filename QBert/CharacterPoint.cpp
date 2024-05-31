#include "PCH.h"
#include "CharacterPoint.h"

CharacterPoint::CharacterPoint()
	:m_NbrPoints()
{
	
}

ECS_CharacterPoint::ECS_CharacterPoint()
	:ECS_Component(true),
	m_NbrPoints()
{

}