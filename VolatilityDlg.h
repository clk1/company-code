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
	Jboolean CaculateVolatility(Jfloat voltage);        //����false����ʾ���ֵ����Сֵ�б仯
	Jboolean getMaxVoltage(DATAID_TYPE id);
	Jboolean setMaxVoltage(Jfloat max,DATAID_TYPE id);
	Jboolean getMinVoltage(DATAID_TYPE id);
	Jboolean setMinVoltage(Jfloat min,DATAID_TYPE id);

	Jboolean submit(Juint8 BJType, DATAID_TYPE BJID);   //��ȡ����ĸ�߲�����

	void getAllValue();

	void sort(s_trans node);                            //�����������

	void showTable();
private:
	Jboolean initDialog();
	DATAID_TYPE m_wStation;
	DATAID_TYPE m_wBJID;
	Juint8 m_byBJType;	

	Jfloat m_Voltage;       //��ǰ��ѹֵ
	Jfloat m_MinVoltage;    //��С��ѹֵ
	Jfloat m_MaxVoltage;    //����ѹֵ

	Jfloat m_Volatility;    //������
	Jfloat m_MaxVolatility; //��󲨶���
	Jfloat m_MinVolatility; //��С������

	listT m_listT;          //�������ĸ�ߵ�

	
};
#endif