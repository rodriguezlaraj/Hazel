#pragma once

#include "Hazel/Core/Core.h"


namespace Hazel {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

//  ##Concatenates two pieces of code
//  # Stringizes the argument. That is, puts it between quotes ""
// All events will need to define the three methods below. All methods are trivial. 
// The concept of a macro helps to edit them all together in the same place if anything changes in the future.
// The GetStaticType function is required because we need to know at runtime which type the KeyPressedEvent is.
// This does not have to be a member funtion. It can be static because we do not need an instance of the class
// to know which type of event it is as it will always be KeyPressed event no matter what the instance is.

// The reason we need a non instance based one, specifically a virtual  function (virtual EventType GetEventType() const override)
// is because if we only have a pointer to an Event class instance, we need to be able to see which type it actually is.

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

// Defines the categories of the instance and also provides an interface to the defined categories
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{

	public:
		bool Handled = false;
		//Methods MUST be defined by inherited classes. The =0 denotes a PURE VIRTUAL function.
		//This means the implementation of the method MUST be defined in the classes derived from Event.
		//The const indicates that the method is not supposed to change data from the class.
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		//Method MAY be defined by inherited classes as there is no =0. The method is virtual.  
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		

		//using keyword just gives an alias.
		//std::function contains almost any object that acts like a fucntion pointer in how you call it
		//Standard function that returns bool and receives T&
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>

		//This has to be called with a function that receives an Event Type Reference and returns a bool
		bool Dispatch(EventFn<T> func)
		{
			//Check if the event type of the event received matches the event type of the class  with which the function was called (i.e. "T")
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				//&m_Event Generic event address
				//(T*)&Event) Cast to the type of the class  with which the function was called (i.e. "T")
				//*(T*)&Event) Dereference of the casted event
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
