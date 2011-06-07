#include "PlusConfigure.h"

#include "vtkBufferedTracker.h"
#include "vtkTracker.h"
#include "vtkTrackerTool.h"
#include "vtkTrackerBuffer.h"

#include "vtkMatrix4x4.h"
#include "vtkFrameToTimeConverter.h"
#include "vtkObjectFactory.h"
#include "vtksys/SystemTools.hxx"


//----------------------------------------------------------------------------
vtkBufferedTracker* vtkBufferedTracker::New()
{
	// First try to create the object from the vtkObjectFactory
	vtkObject* ret = vtkObjectFactory::CreateInstance("vtkBufferedTracker");
	if(ret)
	{
		return (vtkBufferedTracker*)ret;
	}
	// If the factory was unable to create the object, then create it here.
	return new vtkBufferedTracker;
}

//----------------------------------------------------------------------------
vtkBufferedTracker::vtkBufferedTracker()
{
	this->SetNumberOfTools(1); 
	this->ToolNumber = 0; 

	// for accurate timing
	this->Timer = vtkFrameToTimeConverter::New();
	this->Timer->SetNominalFrequency(60.0);
}

//----------------------------------------------------------------------------
vtkBufferedTracker::~vtkBufferedTracker() 
{
	if (this->Tracking)
	{
		this->StopTracking();
	}
	
	if (this->Timer)
	{
		this->Timer->Delete();
	}
}

//----------------------------------------------------------------------------
void vtkBufferedTracker::PrintSelf(ostream& os, vtkIndent indent)
{
	vtkTracker::PrintSelf(os,indent);
}

//----------------------------------------------------------------------------
PlusStatus vtkBufferedTracker::Probe()
{
	if (this->IsTracking())
	{
		return PLUS_SUCCESS;
	}

	return PLUS_SUCCESS; 
} 

//----------------------------------------------------------------------------
PlusStatus vtkBufferedTracker::InternalStartTracking()
{
	if (this->IsTracking())
	{
		return PLUS_SUCCESS;
	}

	// for accurate timing
	this->Timer->Initialize();
	this->Tracking = 1;

	return PLUS_SUCCESS;
}

//----------------------------------------------------------------------------
PlusStatus vtkBufferedTracker::InternalStopTracking()
{
	return PLUS_SUCCESS;
}

//----------------------------------------------------------------------------
PlusStatus vtkBufferedTracker::InternalUpdate()
{
	if (!this->IsTracking())
	{
		return PLUS_FAIL;
	}

  return PLUS_SUCCESS;
}

//----------------------------------------------------------------------------
void vtkBufferedTracker::AddTransform( vtkMatrix4x4* transformMatrix, double timestamp )
{
	long flags=0;

	unsigned long frameNum = this->GetTool(this->ToolNumber)->GetFrameNumber() + 1; 
	// send the transformation matrix and flags to the tool
	this->ToolUpdate(this->ToolNumber, transformMatrix, flags, frameNum, timestamp, timestamp);   
}
