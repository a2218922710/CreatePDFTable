#ifndef CREATEREPORT_H
#define CREATEREPORT_H

#include <QString>
#include <QXmlStreamReader>

class CreateReport
{
    //这个类有几个地方需要翻译一下，比如标题、签名
    //请注意，代码没办法写中文，翻译签名时候需要注意上下两边对齐
public:
    CreateReport();
    //返回报告是否成功生成，参数一：xml路径，参数二：输出路径（暂时只支持输出pdf）
    bool createReportFile(const QString & xmlPath, const QString &outPath);
    //生成的pdf是否带有签名，不调用默认带（不会记住状态，在调用生成报告前调用这个接口）
    void createWithSign(bool isHasSign);

    //方法二 按照行列添加（没写好呢）
    void setItemText(int row,int col ,const QString & text);

    //方法三 按照行添加
    void setIetmElem(const QString &TaskName, const QString &StartTime, const QString EndTime,
                     const QString &SuccessCount, const QString &FailCount, const QString &Total);
    bool savePDFReport(const QString & outPath);
protected:
    struct Elem{
      QString TaskName;
      QString StartTime;
      QString EndTime;
      QString SuccessCount;
      QString FailCount;
      QString Total;
    };

    bool createPDFReport(const QString & outPath);

    QList<Elem> readXMLFile(const QString & xmlFile);
    void readRoot();
    void readDocument();
    void skipUnknownElement();
    void clearElem();

private:
    int m_SuccessTotal;
    int m_FailTotal;
    int m_Total;


    QList<Elem> m_ElemList;
    QXmlStreamReader reader;
    bool m_IsHasSignFlag;

};

#endif // CREATEREPORT_H
