#include "DC_Process.h"


namespace DC_Engine
{
    std::shared_ptr<DC_Process> DC_Process::RemoveChild()
    {
        std::shared_ptr<DC_Process> pChild = m_pChild;
        m_pChild = nullptr;
        return pChild;
    }
    bool DC_Process::OnSuccess()
    {
        if (m_successCallback)
        {
            m_successCallback();
            return true;
        }

        return false;
    }
    bool DC_Process::OnFailed()
    {
        if (m_failedCallback)
        {
            m_failedCallback();
            return true;
        }

        return false;
    }
    bool DC_Process::OnAbort()
    {
        if (m_abortCallback)
        {
            m_abortCallback();
            return true;
        }

        return false;
    }
}
