#ifndef CVOLATILITYDLG_H
#define CVOLATILITYDLG_H

#include <list>

#include "d:\iesbase\obj\mmi\pubmmidll\.ui\dlg_volatility_base.h"
#include "dbi_odbc.h"
#include "qtable.h"

typedef struct _s_Transformer
{
	DATAID_TYPE id;
	char name[NAMELEN];
	Jfloat volatility;
}s_trans;

typedef list<s_trans> listT;

class CVolatilityDlg :
	public dlg_Volatility_Base
{
public:
	CVolatilityDlg(DATAID_TYPE wStation, Juint8 byBJType, DATAID_TYPE wBJID, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);
	~CVolatilityDlg(void);
	Jboolean CaculateVolatility(Jfloat voltage);        //返回false，表示最大值或最小值有变化
	Jboolean getMaxVoltage(DATAID_TYPE id);
	Jboolean setMaxVoltage(Jfloat max,DATAID_TYPE id);
	Jboolean getMinVoltage(DATAID_TYPE id);
	Jboolean setMinVoltage(Jfloat min,DATAID_TYPE id);

	Jboolean submit(Juint8 BJType, DATAID_TYPE BJID);   //获取所有母线波动率

	void getAllValue();

	void sort(s_trans node);                            //排序插入链表

	void showTable();
private:
	Jboolean initDialog();
	DATAID_TYPE m_wStation;
	DATAID_TYPE m_wBJID;
	Juint8 m_byBJType;	

	Jfloat m_Voltage;       //当前电压值
	Jfloat m_MinVoltage;    //最小电压值
	Jfloat m_MaxVoltage;    //最大电压值

	Jfloat m_Volatility;    //波动率
	Jfloat m_MaxVolatility; //最大波动率
	Jfloat m_MinVolatility; //最小波动率

	listT m_listT;          //存放所有母线的

	
};
#endif