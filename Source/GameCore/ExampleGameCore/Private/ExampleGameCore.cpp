#include "ExampleGameCore.h"


DEFINE_LOG_CATEGORY(LogExampleGameCore);

class FExampleGameCore : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FExampleGameCore, ExampleGameCore)

void FExampleGameCore::StartupModule()
{
}

void FExampleGameCore::ShutdownModule()
{
}
