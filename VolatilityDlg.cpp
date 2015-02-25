#include <qlineedit.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qlabel.h>

#include "iescommon.h"
#include "udevicevalue.h"
#include "table_xianlu.h"
#include "global_pub.h"
#include "VolatilityDlg.h"


CVolatilityDlg::CVolatilityDlg(DATAID_TYPE wStation, Juint8 byBJType, DATAID_TYPE wBJID, 
							   QWidget* parent, const char* name, bool modal, WFlags fl) 
							   : dlg_Volatility_Base(parent, name, modal, fl)
{		
	m_wStation=wStation;
	m_byBJType=byBJType;
	m_wBJID=wBJID;

	m_Volatility=0;
	m_MaxVolatility=0;
	m_MinVolatility=0;

	m_Voltage=0;

	getMaxVoltage(m_wBJID);
	getMinVoltage(m_wBJID);

	initDialog();
	
}

CVolatilityDlg::~CVolatilityDlg(void)
{
}

Jboolean CVolatilityDlg::initDialog()
{
	LE_MXName->setText(tr(GetBJName_Mmi(MuXian_ID,m_wBJID).GetBuf()));

	if(0 == m_wStation)
	{
		LE_Station->setText(tr("全网"));
	}
	else
	{
		LE_Station->setText(tr(GetBJName_Mmi(ChangZhan_ID,m_wStation).GetBuf()));
	}
	if(submit(m_byBJType,m_wBJID))
	{
		char szVal[32];
		if(!CaculateVolatility(m_Voltage))      //返回false时，重新获取最大，最小值
		{
			getMaxVoltage(m_wBJID);
			getMinVoltage(m_wBJID);
		}
		sprintf(szVal, "%.3f", m_Voltage);
		LE_Voltage->setText(szVal);
		sprintf(szVal,"%.3f",m_MaxVoltage);
		LE_MaxVoltage->setText(szVal);
		sprintf(szVal,"%.3f",m_MinVoltage);
		LE_MinVoltage->setText(szVal);
		m_Volatility*=100;
		sprintf(szVal,"%.3f%s",m_Volatility,"%");
		LE_Volatility->setText(szVal);
		m_MaxVolatility=((m_MaxVoltage-m_MinVoltage)/220)*100;
		sprintf(szVal,"%.3f%s",m_MaxVolatility,"%");
		LE_MaxVolatility->setText(szVal);
		sprintf(szVal,"%.3f%s",m_MinVolatility,"%");
		LE_MinVolatility->setText(szVal);
	}
	else
	{
		LE_Voltage->setText(tr("未取得"));
		LE_MaxVoltage->setText(tr("未取得"));
		LE_MinVoltage->setText(tr("未取得"));
		LE_Volatility->setText(tr("未取得"));
		LE_MaxVolatility->setText(tr("未取得"));
		LE_MinVolatility->setText(tr("未取得"));
	}
	getAllValue();
	showTable();
	return JTRUE;
}

Jboolean CVolatilityDlg::CaculateVolatility(Jfloat voltage)
{
	Jboolean max_min_change=JTRUE;
	if(voltage>m_MaxVoltage || voltage<m_MinVoltage)
	{
		if(voltage>m_MaxVoltage)
		{
			m_MaxVoltage=voltage;
			m_Volatility=(voltage-m_MinVoltage)/220;
			setMaxVoltage(m_MaxVoltage,m_wBJID);
			max_min_change=JFALSE;
		}
		else
		{
			m_MinVoltage=voltage;
			m_Volatility=(m_MaxVoltage-voltage)/220;
			setMinVoltage(m_MinVoltage,m_wBJID);
			max_min_change=JFALSE;
		}
	}
	else
	{
		Jfloat temp;
		m_Volatility=(m_MaxVoltage-voltage)/220;
		temp=(voltage-m_MinVoltage)/220;
		if(temp>m_Volatility)
		{
			m_Volatility=temp;
		}
	}
	return max_min_change;
}

Jboolean CVolatilityDlg::getMaxVoltage(DATAID_TYPE id)
{
	CIIOdbcDBI* msdbi=GetMSData().GetDatabase();

	Jfloat tmpMax=0;

	if(NULL == msdbi || !msdbi->IsOpen())
	{
		ExtMessageBox::warning(this,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("连接描述库失败"));
		return JFALSE;
	}
	CIIString strsql;
	strsql.Format("select maxvol from volatility where bujianid=%d",id);
	msdbi->PrepareSQL(strsql.GetBuf());
	msdbi->BindCol(IES_SQL_FLOAT,0,&tmpMax);
	msdbi->Exec();
	while(msdbi->Fetch() && !msdbi->IsEnd())
	{
		m_MaxVoltage=tmpMax;
	}
	msdbi->CloseSQL();
	return JTRUE;
}

Jboolean CVolatilityDlg::getMinVoltage(DATAID_TYPE id)
{
	CIIOdbcDBI* msdbi=GetMSData().GetDatabase();

	Jfloat tmpMin=0;
	if(NULL == msdbi || !msdbi->IsOpen())
	{
		ExtMessageBox::warning(this,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("连接描述库失败"));
		return JFALSE;
	}
	CIIString strsql;
	strsql.Format("select minvol from volatility where bujianid=%d",id);
	msdbi->PrepareSQL(strsql.GetBuf());
	msdbi->BindCol(IES_SQL_FLOAT,0,&tmpMin);
	msdbi->Exec();
	while(msdbi->Fetch() && !msdbi->IsEnd())
	{
		m_MinVoltage=tmpMin;
	}
	msdbi->CloseSQL();
	return JTRUE;
}

Jboolean CVolatilityDlg::setMaxVoltage(Jfloat max,DATAID_TYPE id)
{
	CIIOdbcDBI* msdbi=GetMSData().GetDatabase();

	if(NULL == msdbi || !msdbi->IsOpen())
	{
		ExtMessageBox::warning(this,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("连接描述库失败"));
		return JFALSE;
	}
	CIIString strsql;
	strsql.Format("update volatility set maxvol=? where bujianid=%d",id);
	msdbi->PrepareSQL(strsql.GetBuf());
	msdbi->BindPara(IES_SQL_FLOAT,0,&max);
	msdbi->Exec();
	msdbi->CloseSQL();
	return JTRUE;
}

Jboolean CVolatilityDlg::setMinVoltage(Jfloat min,DATAID_TYPE id)
{
	CIIOdbcDBI* msdbi=GetMSData().GetDatabase();

	if(NULL == msdbi || !msdbi->IsOpen())
	{
		ExtMessageBox::warning(this,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("连接描述库失败"));
		return JFALSE;
	}
	CIIString strsql;
	strsql.Format("update volatility set minvol=? where bujianid=%d",id);
	msdbi->PrepareSQL(strsql.GetBuf());
	msdbi->BindPara(IES_SQL_FLOAT,0,&min);
	msdbi->Exec();
	msdbi->CloseSQL();
	return JTRUE;
}

Jboolean CVolatilityDlg::submit(Juint8 BJType, DATAID_TYPE BJID)
{
	CLIENTREQ XLMenu;	//提交定单
	for (Juint8 n=0; n<1; n++)
	{
		XLMenu.IDEntryList[n].bIDEntry[0] = BJType;			//设备种类
#ifndef  DATAIDTYPE_IS_JUINT32
		XLMenu.IDEntryList[n].bIDEntry[2] = LowByte(BJID);	//设备点号
		XLMenu.IDEntryList[n].bIDEntry[3] = HighByte(BJID);	//设备点号
#else
		XLMenu.IDEntryList[n].bIDEntry[2] = LowByte(LowWord(BJID));	//设备点号
		XLMenu.IDEntryList[n].bIDEntry[3] = HighByte(LowWord(BJID));	//设备点号
		XLMenu.IDEntryList[n].bIDEntry[4] = LowByte(HighWord(BJID));	//设备点号
		XLMenu.IDEntryList[n].bIDEntry[5] = HighByte(HighWord(BJID));	//设备点号
#endif
		XLMenu.IDEntryList[n].wEntryLen = sizeof(Jfloat);		//数据长度
		XLMenu.IDEntryList[n].pEntryBuf = NULL;					//数据指针
	}
	XLMenu.IDEntryList[0].bIDEntry[1] = MX_VabX;

	XLMenu.wCountOfEntry = 7;
	XLMenu.wSheetCode = 31;
	XLMenu.lpReqCallBack = (LP_SBCPLT_ROUTINE)GlobalSheetCallback;
	XLMenu.dwUseAddPara = 0;
	if (!DB_SubmitSheet(&XLMenu))
	{
		ExtMessageBox::warning(this, tr("错误"), tr("线路数据定单提交失败!"));
		return JFALSE;
	}
	if (waitSheetBack(&XLMenu))
	{
		UDeviceValue	uVal;
		if (XLMenu.IDEntryList[0].wNewMark == SB_NpREQ_NEW
			&& XLMenu.IDEntryList[0].pEntryBuf)
		{
			memcpy(&uVal, XLMenu.IDEntryList[0].pEntryBuf, sizeof(UDeviceValue));
			m_Voltage=networktohost(uVal.m_tm);
			return JTRUE;
		}
	}
	DB_CancelSheet(&XLMenu);
	return JFALSE;
}

void CVolatilityDlg::getAllValue()
{
	CIIOdbcDBI* msdbi=GetMSData().GetDatabase();

	if(NULL == msdbi || !msdbi->IsOpen())
	{
		ExtMessageBox::warning(this,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("连接描述库失败"));
		return;
	}
	

	s_trans tranNode;
	memset(&tranNode,0,sizeof(&tranNode));

	listT listMuxian;

	CIIString strsql;
	strsql.Format("select id,mingzi from muxian");
	msdbi->PrepareSQL(strsql.GetBuf());
	msdbi->BindCol(SQL_DATAID_TYPE,0,&tranNode.id);
	msdbi->BindCol(IES_SQL_STRING,NAMELEN,&tranNode.name);
	msdbi->Exec();
	while(msdbi->Fetch() && !msdbi->IsEnd())
	{
		/*if(submit(m_byBJType,tranNode.id))
		{
			CaculateVolatility(m_Voltage);
			tranNode.volatility=m_Volatility;
			sort(tranNode);
		}*/
		listMuxian.push_back(tranNode);
	}

	CIITime timeNow;				
	timeNow.SetCurrentTime();	//根据当前时间，查找最新存盘的电压大小
	int minute=(timeNow.GetMinute())/15;  //15分钟进行存点数据，用序号表示15分钟间隔内的数据
	int hour=timeNow.GetHour();
	char hourLine[10];
	sprintf(hourLine,"%dh",hour);
	memset(&tranNode,0,sizeof(&tranNode));

	listT::iterator it;
	for(it=listMuxian.begin();it!=listMuxian.end();it++)
	{
		tranNode.id=it->id;
		tranNode.name=it->name;

	}


}

void CVolatilityDlg::sort(s_trans node)
{
	if(m_listT.empty())
	{
		m_listT.push_back(node);
	}
	else
	{
		listT::iterator it;
		for(it=m_listT.begin();it!=m_listT.end();it++)
		{
			if(it->volatility<node.volatility)
			{
				m_listT.insert(it,node);
				return;
			}
		}
	}

}

void CVolatilityDlg::showTable()
{
	listT::iterator it;

	int Tsize=m_listT.size();
	VoTable->setNumRows(Tsize);
	int row=0;
	for(it=m_listT.begin();it!=m_listT.end();it++)
	{
		VoTable->setText(row,0,QString::number(it->id));
		VoTable->setText(row,1,QString::fromLocal8Bit(it->name));
		VoTable->setText(row,2,QString::number(it->volatility));
		row++;
	}
}