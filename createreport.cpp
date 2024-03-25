#include "createreport.h"

#include <QFile>
#include <QDebug>
#include <QPrinter>
#include <QTextDocument>
#include <QDateTime>
#include <QStringList>
#include <QTextBlock>
#include <QDir>

CreateReport::CreateReport()
{
    clearElem();
}

bool CreateReport::createReportFile(const QString &xmlPath, const QString &outPath)
{
    QFile file(xmlPath);
    if(!file.exists())
    {
        return false;
    }
    QList<Elem> ElemList = readXMLFile(xmlPath);
    if(ElemList.isEmpty())
    {
        return false;
    }
    bool isCreateReport = createPDFReport(outPath);
    return isCreateReport;
}

void CreateReport::createWithSign(bool isHasSign)
{
    m_IsHasSignFlag = isHasSign;
}

void CreateReport::setItemText(int row, int col, const QString &text)
{
    if(row > m_ElemList.size())
    {
        int needElemCou = row - m_ElemList.size();
        for(int i = 0; i < needElemCou; ++i)
        {
            Elem elem;
            m_ElemList << elem;
        }
        Elem elem = m_ElemList[row - 1];
        switch (col) {
        case 1:
            elem.TaskName = text;
            break;
        case 2:
            elem.StartTime = text;
            break;
        case 3:
            elem.EndTime = text;
            break;
        case 4:
            elem.SuccessCount = text;
            m_SuccessTotal += QString(text).toInt();
            break;
        case 5:
            elem.FailCount = text;
            m_FailTotal += QString(text).toInt();
            break;
        case 6:
            elem.Total = text;
            m_Total += QString(text).toInt();
            break;
        }
        m_ElemList[row - 1] = elem;
    }else{
        Elem elem = m_ElemList[row - 1];
        switch (col) {
        case 1:
            elem.TaskName = text;
            break;
        case 2:
            elem.StartTime = text;
            break;
        case 3:
            elem.EndTime = text;
            break;
        case 4:
            elem.SuccessCount = text;
            m_SuccessTotal += QString(text).toInt();
            break;
        case 5:
            elem.FailCount = text;
            m_FailTotal += QString(text).toInt();
            break;
        case 6:
            elem.Total = text;
            m_Total += QString(text).toInt();
            break;
        }
        m_ElemList[row - 1] = elem;
    }
}

void CreateReport::setIetmElem(const QString &TaskName, const QString &StartTime, const QString EndTime, const QString &SuccessCount, const QString &FailCount, const QString &Total)
{
    Elem elem;
    elem.TaskName = TaskName;
    elem.StartTime = StartTime;
    elem.EndTime = EndTime;
    elem.SuccessCount = SuccessCount;
    elem.FailCount = FailCount;
    elem.Total = Total;
    m_ElemList << elem;
}

bool CreateReport::savePDFReport(const QString &outPath)
{
    return createPDFReport(outPath);
}

bool CreateReport::createPDFReport(const QString &outPath)
{
    QString outFile = outPath;
#ifdef Q_OS_LINUX
    outFile = QDir::fromNativeSeparators(outFile);
#else
    outFile = QDir::toNativeSeparators(outFile);
#endif

    qDebug()<<outFile;

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
    QStringList title;
    title.push_back(QObject::tr("Serial Number"));
    title.push_back(QObject::tr("Task Name"));
    title.push_back(QObject::tr("Start Time"));
    title.push_back(QObject::tr("Finish Time"));
    title.push_back(QObject::tr("Success Count"));
    title.push_back(QObject::tr("Fail Count"));
    title.push_back(QObject::tr("All"));
    QString html;
    html += "<h1 align=\"center\">" + QObject::tr("OFD Conversion Report")+"</h1>";
//    html += "<table width=\"600\" border=\"1\" align=\"center\" border-style=\"solid\" bordercolor=\"black\">";
    html += "<table width=\"100%\" align=\"center\" border=\"1\" cellspacing=\"0\" cellpadding=\"0\" style=\"width: 100%; height: 100%;\">";
    html += "<tr style=\"background-color:white\">";
    for(int i = 0; i < title.count(); ++i)
    {
        html += QString("<th align=\"center\">%1</th>").arg(title.at(i));
    }
    html += "</tr>";
    int count = m_ElemList.size();
    for(int i = 0; i < m_ElemList.size(); ++i)
    {
        html += "<tr>";
        Elem elem = m_ElemList[i];
        html += QString("<td align=\"center\">%1</td>").arg(QString::number( i + 1));
        html += QString("<td align=\"center\">%1</td>").arg(elem.TaskName);
        html += QString("<td align=\"center\">%1</td>").arg(elem.StartTime);
        html += QString("<td align=\"center\">%1</td>").arg(elem.EndTime);
        html += QString("<td align=\"center\">%1</td>").arg(elem.SuccessCount);
        html += QString("<td align=\"center\">%1</td>").arg(elem.FailCount);
        html += QString("<td align=\"center\">%1</td>").arg(elem.Total);
        html += "</tr>";

        m_SuccessTotal += QString(elem.SuccessCount).toInt();
        m_FailTotal += QString(elem.FailCount).toInt();
        m_Total += QString(elem.Total).toInt();
    }

    html += "<tr>";
    html += QString("<td align=\"center\">%1</td>").arg(QObject::tr("TotalCount"));
    html += QString("<td align=\"center\">%1</td>").arg("/");
    html += QString("<td align=\"center\">%1</td>").arg("/");
    html += QString("<td align=\"center\">%1</td>").arg("/");
    html += QString("<td align=\"center\">%1</td>").arg(QString::number(m_SuccessTotal));
    html += QString("<td align=\"center\">%1</td>").arg(QString::number(m_FailTotal));
    html += QString("<td align=\"center\">%1</td>").arg(QString::number(m_Total));
    html += "</tr>";

    clearElem();

    html += "</table>";
    html += "<p align=\"right\">" + QString(QObject::tr("Report Create Time: ")+current_date) + "</p>";

    if(m_IsHasSignFlag)
    {
        count = (count + 4) % 51;
        if(count < 2)
        {
            count = 48;
        }else{
            count = 51 - count;
        }

        for(int i = 0; i < count; ++i)
        {
            html += "<br/>";
        }


        html += "<div align=\"right\" vertical-align=\"bottom\">Approver_____________</div>";
        html += "<br/>";
        html += "<div align=\"right\" vertical-align=\"bottom\">Data_________________</div>";
    }

    QPrinter printer_html;
    printer_html.setPageSize(QPrinter::A4);
    printer_html.setOutputFormat(QPrinter::PdfFormat);
    printer_html.setOutputFileName(outFile);
    QTextDocument text_document;
    text_document.setHtml(html);
    text_document.print(&printer_html);
    text_document.end();

    QFile file(outFile);
    if(file.exists())
    {
        return true;
    }else{
        return false;
    }
}

QList<CreateReport::Elem> CreateReport::readXMLFile(const QString &xmlFile)
{
    QFile file(xmlFile);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return QList<CreateReport::Elem>();
    }
    reader.setDevice(&file);
    while (!reader.atEnd())
    {
        if (reader.isStartElement())
        {
            if (reader.name() == "root")
            {
                readRoot();//递归下降算法，层层读取
            }else{
                return QList<CreateReport::Elem>();
            }
        }
        else
        {
            reader.readNext(); //循坏调用首次移动3次，后面移动一次
        }
    }
    file.close();
    if (reader.hasError())
    {
        return QList<CreateReport::Elem>();
    }
    return m_ElemList;

}

void CreateReport::readRoot()
{
    reader.readNext(); // 读取下一个记号，它返回记号的类型
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }

        if (reader.isStartElement())
        {
            if (reader.name() == "document")
            {
                readDocument();
            }
            else
            {
                skipUnknownElement();
            }
        }
        else
        {
            reader.readNext();
        }
    }
}

void CreateReport::readDocument()
{
    reader.readNext();
    Elem tempELem;
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }

        if (reader.name() == "document")
        {
            readDocument();
        }
        else if(reader.name() == "taskname")
        {
            tempELem.TaskName = reader.readElementText();
//            qDebug()<<reader.readElementText();
            reader.readNext();

        }else if(reader.name() == "starttime")
        {
            tempELem.StartTime = reader.readElementText();
//            qDebug()<<reader.readElementText();
            reader.readNext();

        }else if(reader.name() == "endtime")
        {
            tempELem.EndTime = reader.readElementText();
//            qDebug()<<reader.readElementText();
            reader.readNext();

        }else if(reader.name() == "successcount")
        {
            tempELem.SuccessCount = reader.readElementText();
//            qDebug()<<reader.readElementText();
            reader.readNext();

        }else if(reader.name() == "failcount")
        {
            tempELem.FailCount = reader.readElementText();;
//            qDebug()<<reader.readElementText();
            reader.readNext();
        }else if(reader.name() == "total")
        {
            tempELem.Total = reader.readElementText();;
//            qDebug()<<reader.readElementText();
            reader.readNext();
        }else{
            reader.readNext();
        }
    }
    m_ElemList.push_back(tempELem);
}

void CreateReport::skipUnknownElement()
{
    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }

        if (reader.isStartElement())
        {
            skipUnknownElement();
        }
        else
        {
            reader.readNext();
        }
    }
}

void CreateReport::clearElem()
{
    m_IsHasSignFlag = true;
    m_ElemList.clear();
    m_SuccessTotal = 0;
    m_FailTotal = 0;
    m_Total = 0;
}
