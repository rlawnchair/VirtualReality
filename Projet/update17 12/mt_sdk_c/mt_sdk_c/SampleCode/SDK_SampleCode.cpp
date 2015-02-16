//+---------------------------------------------------------------------------
//
//  PQLabs.
//
//  Copyright (c) PQLabs.  All rights reserved.
//
//  File:       SDK_SampleCode.cpp
//
//  Contents:   Implementation of SDK_SampleCode.h
//
//	Date:		2008-12-19
//
//----------------------------------------------------------------------------

#include "stdafx.h"
#include "SDK_SampleCode.h"

#include <iostream>
#include <set>
#include <map>
#include <cassert>
#include <functional>
using namespace std;

/*extern*/ float mouseX;
/*extern*/ float mouseY;
 float mouseDraggingX = 0;
 float mouseDraggingY = 0;

 bool mouving = false;

  float mouseDraggingStartX = 0;
 float mouseDraggingStartY = 0;

   float mouseDraggingEndX = 0;
 float mouseDraggingEndY = 0;

 float sphereX = 0;
 float sphereY = 0;

namespace PQ_SDK_MultiTouchSample
{

Sample::Sample()
{
	memset(m_pf_on_tges,0, sizeof(m_pf_on_tges));
}
Sample::~Sample()
{
	DisconnectServer();
}
/////////////////////////// functions ///////////////////////////////////
int Sample::Init()
{
	int err_code = PQMTE_SUCCESS;

	// initialize the handle functions of gestures;
	InitFuncOnTG();
	// set the functions on server callback
	SetFuncsOnReceiveProc();
	// connect server
	cout << " connect to server..." << endl;
	if((err_code = ConnectServer()) != PQMTE_SUCCESS){
		cout << " connect server fail, socket error code:" << err_code << endl;
		return err_code;
	}
	// send request to server
	cout << " connect success, send request." << endl;
	TouchClientRequest tcq = {0};
	tcq.type = RQST_RAWDATA_ALL | RQST_GESTURE_ALL;
	if((err_code = SendRequest(tcq)) != PQMTE_SUCCESS){
		cout << " send request fail, error code:" << err_code << endl;
		return err_code;
	}
	////////////you can set the move_threshold when the tcq.type is RQST_RAWDATA_INSIDE;
	////send threshold
	//int move_threshold = 0;// 0 pixel, receuve all the touch points that touching in the windows area of this client;
	//if((err_code = SendThreshold(move_threshold)) != PQMTE_SUCCESS){
	//	cout << " send threadhold fail, error code:" << err_code << endl;
	//	return err_code;
	//}
	
	//////// you can set the resolution of the touch point(raw data) here;
	//// setrawdata_resolution
	//int maxX = 32768, maxY = 32768;
	//if((err_code= SetRawDataResolution(maxX, maxY)) != PQMTE_SUCCESS){
	//	cout << " set raw data resolution fail, error code:" << err_code << endl;
	//}
	////////////////////////
	//get server resolution
	if((err_code = GetServerResolution(OnGetServerResolution, NULL)) != PQMTE_SUCCESS){
		cout << " get server resolution fail,error code:" << err_code << endl;
		return err_code;
	}
	//
	// start receiving
	cout << " send request success, start recv." << endl;
	return err_code;
}

void Sample:: InitFuncOnTG()
{
	// initialize the call back functions of toucha gestures;
	m_pf_on_tges[TG_TOUCH_START] = &Sample::OnTG_TouchStart;
	m_pf_on_tges[TG_DOWN] = &Sample::OnTG_Down;
	m_pf_on_tges[TG_MOVE] = &Sample::OnTG_Move;
	m_pf_on_tges[TG_UP] = &Sample::OnTG_Up;

	m_pf_on_tges[TG_SECOND_DOWN] = &Sample::OnTG_SecondDown;
	m_pf_on_tges[TG_SECOND_UP] = &Sample::OnTG_SecondUp;

	m_pf_on_tges[TG_SPLIT_START] = &Sample::OnTG_SplitStart;
	m_pf_on_tges[TG_SPLIT_APART] = &Sample::OnTG_SplitApart;
	m_pf_on_tges[TG_SPLIT_CLOSE] = &Sample::OnTG_SplitClose;
	m_pf_on_tges[TG_SPLIT_END] = &Sample::OnTG_SplitEnd;

	m_pf_on_tges[TG_TOUCH_END] = &Sample::OnTG_TouchEnd;
}
void Sample::SetFuncsOnReceiveProc()
{
	PFuncOnReceivePointFrame old_rf_func = SetOnReceivePointFrame(&Sample::OnReceivePointFrame,this);
	PFuncOnReceiveGesture old_rg_func = SetOnReceiveGesture(&Sample::OnReceiveGesture,this);
	PFuncOnServerBreak old_svr_break = SetOnServerBreak(&Sample::OnServerBreak,NULL);
	PFuncOnReceiveError old_rcv_err_func = SetOnReceiveError(&Sample::OnReceiveError,NULL);
	PFuncOnGetDeviceInfo old_gdi_func = SetOnGetDeviceInfo(&Sample::OnGetDeviceInfo,NULL);
}

void Sample:: OnReceivePointFrame(int frame_id, int time_stamp, int moving_point_count, const TouchPoint * moving_point_array, void * call_back_object)
{
	Sample * sample = static_cast<Sample*>(call_back_object);
	assert(sample != NULL);
	const char * tp_event[] = 
	{
		"down",
		"move",
		"up",
	};
	
	cout << " frame_id:" << frame_id << " time:"  << time_stamp << " ms" << " moving point count:" << moving_point_count << endl;
	for(int i = 0; i < moving_point_count; ++ i){
		TouchPoint tp = moving_point_array[i];
		sample->OnTouchPoint(tp);
	}
	//throw exception("test exception here");
}
void Sample:: OnReceiveGesture(const TouchGesture & ges, void * call_back_object)
{
	Sample * sample = static_cast<Sample*>(call_back_object);
	assert(sample != NULL);
	sample->OnTouchGesture(ges);
	//throw exception("test exception here");
}
void Sample:: OnServerBreak(void * param, void * call_back_object)
{
	// when the server break, disconenct server;
	cout << "server break, disconnect here" << endl;
	DisconnectServer();
}
void Sample::OnReceiveError(int err_code, void * call_back_object)
{
	switch(err_code)
	{
	case PQMTE_RCV_INVALIDATE_DATA:
		cout << " error: receive invalidate data." << endl;
		break;
	case PQMTE_SERVER_VERSION_OLD:
		cout << " error: the multi-touch server is old for this client, please update the multi-touch server." << endl;
		break;
	case PQMTE_EXCEPTION_FROM_CALLBACKFUNCTION:
		cout << "**** some exceptions thrown from the call back functions." << endl;
		assert(0); //need to add try/catch in the callback functions to fix the bug;
		break;
	default:
		cout << " socket error, socket error code:" << err_code << endl;
	}
}
void Sample:: OnGetServerResolution(int x, int y, void * call_back_object)
{
	cout << " server resolution:" << x << "," << y << endl;
}
void Sample::OnGetDeviceInfo(const TouchDeviceInfo & deviceinfo,void *call_back_object)
{
	cout << " touch screen, SerialNumber: " << deviceinfo.serial_number <<",(" << deviceinfo.screen_width << "," << deviceinfo.screen_height << ")."<<  endl;
}
// here, just record the position of point,
//	you can do mouse map like "OnTG_Down" etc;
void Sample:: OnTouchPoint(const TouchPoint & tp)
{
	switch(tp.point_event)
	{
	case TP_DOWN:{
		cout << "  point " << tp.id << " come at (" << tp.x << "," << tp.y 
			<< ") width:" << tp.dx << " height:" << tp.dy << endl;

		break;
				 }
	case TP_MOVE:
		{
		cout << "  point " << tp.id << " move at (" << tp.x << "," << tp.y 
			<< ") width:" << tp.dx << " height:" << tp.dy << endl;

		 mouseDraggingX = tp.dx;
         mouseDraggingY = tp.dy;

		break;
		}
	case TP_UP:
		cout << "  point " << tp.id << " leave at (" << tp.x << "," << tp.y 
			<< ") width:" << tp.dx << " height:" << tp.dy << endl;
		break;
	}

}
void Sample:: OnTouchGesture(const TouchGesture & tg)
{
	if(TG_NO_ACTION == tg.type)
		return ;
	
	assert(tg.type <= TG_TOUCH_END);
	DefaultOnTG(tg,this);
	PFuncOnTouchGesture pf = m_pf_on_tges[tg.type];
	if(NULL != pf){
		pf(tg,this);
	}
}
void Sample:: OnTG_TouchStart(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_TOUCH_START);
	cout << "  here, the touch start, initialize something." << endl;
}
void Sample:: DefaultOnTG(const TouchGesture & tg,void * call_object) // just show the gesture
{
	cout <<"ges,name:"<< GetGestureName(tg) << " type:" << tg.type << ",param size:" << tg.param_size << " ";
	for(int i = 0; i < tg.param_size; ++ i)
		cout << tg.params[i] << " ";
	cout << endl;
}
void Sample:: OnTG_Down(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_DOWN && tg.param_size >= 2);
	cout << "  the single finger touching at :( " 
		<< tg.params[0] << "," << tg.params[1] << " )" << endl;
		
			mouseX = mouseDraggingStartX = tg.params[0];
			mouseY = mouseDraggingStartY = tg.params[1];
}
void Sample:: OnTG_Move(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_MOVE && tg.param_size >= 2);
	cout << "  the single finger moving on the screen at :( " 
		<< tg.params[0] << "," << tg.params[1] << " )" << endl;

	mouseX = mouseDraggingEndX = tg.params[0];
	mouseY = mouseDraggingEndY = tg.params[1];

	mouving = true;
}
void Sample:: OnTG_Up(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_UP && tg.param_size >= 2);
	cout << " the single finger is leaving the screen at :( " 
		<< tg.params[0] << "," << tg.params[1] << " )" << endl;
	mouving = false;
}
//
void Sample:: OnTG_SecondDown(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SECOND_DOWN && tg.param_size >= 4);
	cout << "  the second finger touching at :( " 
		<< tg.params[0] << "," << tg.params[1] << " ),"
		<< " after the first finger touched at :( "
		<< tg.params[2] << "," << tg.params[3] << " )" << endl;

		mouseX = tg.params[2];
		mouseX = tg.params[3];
}
void Sample:: OnTG_SecondUp(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SECOND_UP && tg.param_size >= 4);
	cout << "  the second finger is leaving at :( " 
		<< tg.params[0] << "," << tg.params[1] << " ),"
		<< " while the first finger still anchored around :( "
		<< tg.params[2] << "," << tg.params[3] << " )" << endl;

			sphereX = tg.params[0];
		sphereY = tg.params[1];
}
//
void Sample:: OnTG_SplitStart(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_START && tg.param_size >= 4);
	cout << "  the two fingers is splitting with one finger at: ( " 
		<< tg.params[0] << "," << tg.params[1] << " ),"
		<< " , the other at :( "
		<< tg.params[2] << "," << tg.params[3] << " )" << endl;
}

void Sample:: OnTG_SplitApart(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_APART && tg.param_size >= 1);
	cout << "  the two fingers is splitting apart with there distance incresed by " 
		<< tg.params[0]
		<< " with a ratio :" << tg.params[1]
		<< endl;
}
void Sample:: OnTG_SplitClose(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_CLOSE && tg.param_size >= 1);
	cout << "  the two fingers is splitting close with there distance decresed by " 
		<< tg.params[0]
		<< " with a ratio :" << tg.params[1]
		<< endl;
}
void Sample:: OnTG_SplitEnd(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_SPLIT_END);
	cout << "  the two splitting fingers with one finger at: ( " 
		<< tg.params[0] << "," << tg.params[1] << " ),"
		<< " , the other at :( "
		<< tg.params[2] << "," << tg.params[3] << " )" 
		<< " will end" << endl;
}
// OnTG_TouchEnd: to clear what need to clear
void Sample:: OnTG_TouchEnd(const TouchGesture & tg,void * call_object)
{
	assert(tg.type == TG_TOUCH_END);
	cout << "  all the fingers is leaving and there is no fingers on the screen." << endl;
}
/////////////////////////// functions ///////////////////////////////////

}