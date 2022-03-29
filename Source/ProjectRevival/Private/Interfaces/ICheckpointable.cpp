// Project Revival. All Rights Reserved


#include "Interfaces/ICheckpointable.h"

// Add default functionality here for any IICheckpointable functions that are not pure virtual.
AActor* IICheckpointable::GetPlayerStartForCheckpoint()
{
	return nullptr;
}

bool IICheckpointable::HasName(FName CheckpointName)
{
	return false;
}

bool IICheckpointable::IsFirstCheckpointOnMap()
{
	return false;
}
