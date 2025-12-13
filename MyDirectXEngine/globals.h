#pragma once

#define VERIFY(result, failMessage) if(FAILED(result)) throw failMessage
#define DOZERO(structure) ZeroMemory(&structure, sizeof(structure))
#define STRUCT(TYPE, NAME) TYPE NAME; DOZERO(NAME)