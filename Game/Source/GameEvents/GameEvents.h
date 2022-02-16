#pragma once

//===============================================
// RemoveFromGameEvent class
//===============================================
class RemoveFromGameEvent : public fw::Event
{
public:
    RemoveFromGameEvent(fw::GameObject* pObject)
    {
        m_pObject = pObject;
    }
    virtual ~RemoveFromGameEvent() {}

    // Event Type Getters.
    static const char* GetStaticEventType() { return "RemoveFromGameEvent"; }
    virtual const char* GetEventType() override { return GetStaticEventType(); }

    // Getters.
    fw::GameObject* GetGameObject() { return m_pObject; }

protected:
    fw::GameObject* m_pObject = nullptr;
};

class SceneChangeEvent : public fw::Event
{
protected:
    std::string m_sceneName;
public:
    SceneChangeEvent(std::string sceneName) : m_sceneName(sceneName) {}

    static const char* GetStaticEventType() { return "SceneChangeEvent"; }
    virtual const char* GetEventType() override { return GetStaticEventType(); }

    std::string GetSceneName() { return m_sceneName; }

};

class OpenFileEvent : public fw::Event
{
protected:
	char*  m_filename;
public:
	OpenFileEvent(char* filename) : m_filename(filename) {}

	static const char* GetStaticEventType() { return "OpenFileEvent"; }
	virtual const char* GetEventType() override { return GetStaticEventType(); }

	char* GetFilename() { return m_filename; }

};
