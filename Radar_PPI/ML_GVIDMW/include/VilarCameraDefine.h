#pragma once

struct _CAMERA_CONNECT_INFO
{
	char url[ 1024 ];
	unsigned short port;
	char name[ 48 ];
	char pwd[ 48 ];
	void * additionalInfo;

	_CAMERA_CONNECT_INFO() : additionalInfo( NULL )
	{
	}
};

struct _HTTP_ADDITIONAL_INFO
{
	char * type;
	char * text;
	char * streamLocation;

	_HTTP_ADDITIONAL_INFO() : type ( NULL ), text( NULL ), streamLocation( "/cgi-bin/sf.cgi" )
	{
	}
};

struct _CAMERA_VIDEO_INFO
{
	unsigned int width;
	unsigned int height;
	unsigned int fps;
};
