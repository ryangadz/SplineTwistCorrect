// Copyright 2018 Ryan Gadz, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FSplineTwistCorrectModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};