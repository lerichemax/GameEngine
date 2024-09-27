#pragma once
#include <memory>

#include "NapoleonEngine.h"

class SingletonWrapper
{
public:
	virtual ~SingletonWrapper() = default;
protected:
	SingletonWrapper() = default;
};

template <typename T>
class Singleton : public SingletonWrapper
{
public:
	static T& Get()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T{};
			NapoleonEngine::GetEngine()->RegisterSingleton(m_pInstance);
		}

		return *m_pInstance;
	}

	~Singleton()
	{
		m_pInstance = nullptr;
	};

	Singleton(const Singleton& other) = delete;
	Singleton(Singleton&& other) = delete;
	Singleton& operator=(const Singleton& other) = delete;
	Singleton& operator=(Singleton&& other) = delete;

protected:
	Singleton() = default;

private:
	friend class NapoleonEngine;
	static T* m_pInstance;

};

template <typename T>
T* Singleton<T>::m_pInstance{};