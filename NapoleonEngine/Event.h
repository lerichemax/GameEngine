#pragma once
#include <functional>

template<class T, typename... Types>
class EventHandler final
{
	friend T;
	using Subscriber = std::function<void(Types...)>;

public:
	void Subscribe(Subscriber func);
	void Unsuscribe(Subscriber func);
	void UnsuscribeAll();

private:
	std::vector<Subscriber> m_Subscribers;

	void Notify(Types...) const;
}; 

template<class T, typename... Types>
void EventHandler<T, Types...>::Notify(Types... args) const
{
	for (auto sub : m_Subscribers)
	{
		sub(args...);
	}
}

template<class T, typename... Types>
void EventHandler<T, Types...>::Subscribe(Subscriber func)
{
	auto subscriberIterator = std::find_if(m_Subscribers.begin(), m_Subscribers.end(), [&func](Subscriber const& sub) {
		return sub.target_type() == func.target_type() && sub.target<void(Types...)>() == func.target<void(Types...)>();
		});

	if (subscriberIterator != m_Subscribers.end())
	{
		LOG_INFO("Subscriber already registered")
		return;
	}

	m_Subscribers.push_back(func);
}

template<class T, typename... Types>
void EventHandler<T, Types...>::Unsuscribe(Subscriber func)
{
	m_Subscribers.erase(std::remove_if(m_Subscribers.begin(), m_Subscribers.end(), [func](Subscriber const& sub) {
		return sub.target<void(Types...)>() == func.target<void(Types...)>();
		}), m_Subscribers.end());
}

template<class T, typename... Types>
void EventHandler<T, Types...>::UnsuscribeAll()
{
	m_Subscribers.clear();
}