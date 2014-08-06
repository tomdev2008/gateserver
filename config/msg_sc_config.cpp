/*
 * msg_sc_config.cpp
 *
 *  Created on: 2013-1-11
 *      Author: luocj
 */

#include "msg_sc_config.h"
#include "tinyxml/tinyxml.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CSCMsgConfig::Initialize(const char* szFileName, const int32_t type)
{
	arrMsgInfo.clear();
	//¼ÓÔØÅäÖÃÎÄ¼þ
	TiXmlDocument doc(DEFAULT_SC_MSG_CONFIGFILENAME);
	if (!doc.LoadFile(TIXML_ENCODING_UTF8))
	{
		WRITE_ERROR_LOG("load file failed! {file name=%s}\n", szFileName);
		return -1;
	}

	TiXmlElement *pRoot = doc.RootElement();
	if (NULL == pRoot)
	{
		WRITE_ERROR_LOG("load root failed! {file name=%s}\n", szFileName);
		return -1;
	}

	const char* pszValue = NULL;

	TiXmlElement *pMsg = pRoot->FirstChildElement("msg");
	if (NULL == pMsg)
	{
		WRITE_ERROR_LOG("load node filter failed! {file name=%s}\n", szFileName);
		return -1;
	}
	while(NULL != pMsg)
	{
		pszValue = pMsg->Attribute("msg_id");
		if (NULL == pszValue)
		{
			WRITE_ERROR_LOG("read attr msg id failed!\n");
			return -1;
		}
		int32_t id = 0;
		sscanf(pszValue, "0x%08X", &id);
		uint32_t nMsgID = (uint32_t)id;

		arrMsgInfo[nMsgID].nMsgID = nMsgID;

		WRITE_NOTICE_LOG("read filter success! {msg id=0x%04X}\n",
			nMsgID);

		pMsg = pMsg->NextSiblingElement();
	}
	return S_OK;
}

int32_t CSCMsgConfig::Reload(const char* szFileName, const int32_t type )
{
	return Initialize(szFileName,type);
}

int32_t CSCMsgConfig::Uninitialize()
{
	return S_OK;
}

FRAME_GATESERVER_NAMESPACE_END
