#pragma once
#include "Observer.h"
#include "Pyramid.h"

enum class QubeEvents
{
	PlayerJump,
	QubeFlipped,
	DiskUsed
};

class QBert;
class QubeObserver final : public empire::Observer
{
public:
	QubeObserver(Pyramid* const pController, QBert* const pQBert);
	~QubeObserver() = default;
	void Notify(empire::Component * object, int event) override;
private:
	Pyramid * const m_pPyramid;
	QBert* const m_pQBert;
};