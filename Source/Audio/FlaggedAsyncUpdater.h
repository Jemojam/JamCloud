

#pragma once

class FlaggedAsyncUpdater : public AsyncUpdater
{
public:
	//==============================================================================
	void markAndUpdate(bool& flag) { flag = true; triggerAsyncUpdate(); }

	bool compareAndReset(bool& flag) noexcept
	{
		if (!flag)
			return false;

		flag = false;
		return true;
	}
};