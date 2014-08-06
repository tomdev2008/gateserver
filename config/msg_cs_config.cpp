/*
 * msg_cs_config.cpp
 *
 *  Created on: 2013-1-10
 *      Author: luocj
 */

#include "msg_cs_config.h"
#include "tinyxml/tinyxml.h"

FRAME_GATESERVER_NAMESPACE_BEGIN

int32_t CCSMsgConfig::Initialize(const char* szFileName, const int32_t type)
{
	arrMsgInfo.clear();
	//¼ÓÔØÅäÖÃÎÄ¼þ
	TiXmlDocument doc(DEFAULT_CS_MSG_CONFIGFILENAME);
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

		int32_t type = 0;
		pszValue = pMsg->Attribute("entity_type", &type);
		if (NULL == pszValue)
		{
			WRITE_ERROR_LOG("read attr entity type failed!\n");
			return -1;
		}

		int32_t nTransType = enmTransType_None;
		pszValue = pMsg->Attribute("trans_type", &nTransType);
		if (NULL == pszValue)
		{
			WRITE_ERROR_LOG("read attr trans_type failed!\n");
			return -1;
		}

		arrMsgInfo[nMsgID].nMsgID = nMsgID;
		arrMsgInfo[nMsgID].nToEntityType = (EntityType)type;
		arrMsgInfo[nMsgID].nTransType =  nTransType;

		WRITE_NOTICE_LOG("read filter success! {msg id=0x%04X, entity type=%d}\n",
			nMsgID, (EntityType)type);

		pMsg = pMsg->NextSiblingElement();
	}
	return S_OK;
}

int32_t CCSMsgConfig::Reload(const char* szFileName, const int32_t type )
{
	return Initialize(szFileName,type);
}

int32_t CCSMsgConfig::Uninitialize()
{
	return S_OK;
}

EntityType CCSMsgConfig::GetEntityTypeByMsgId(uint32_t nMsgID)
{
	return arrMsgInfo[nMsgID].nToEntityType;
}

TransType CCSMsgConfig::GetTransTypeByMsgId(uint32_t nMsgID)
{
	return arrMsgInfo[nMsgID].nTransType;
}


FRAME_GATESERVER_NAMESPACE_END
