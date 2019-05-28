#ifndef SINXMLHIGHLIGHTER_H
#define SINXMLHIGHLIGHTER_H


#include <QSyntaxHighlighter>
#include <QTextEdit>

class SinXmlHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	SinXmlHighlighter(QObject * parent);
	SinXmlHighlighter(QTextDocument * parent);
	SinXmlHighlighter(QTextEdit * parent);

protected:
	virtual void highlightBlock(const QString & text);

private:
	void highlightByRegex(const QTextCharFormat & format,
		const QRegExp & regex, const QString & text);

	void setRegexes();
	void setFormats();

private:
	QTextCharFormat     m_xmlKeywordFormat;
	QTextCharFormat     m_xmlElementFormat;
	QTextCharFormat     m_xmlAttributeFormat;
	QTextCharFormat     m_xmlValueFormat;
	QTextCharFormat     m_xmlCommentFormat;

	QList<QRegExp>      m_xmlKeywordRegexes;
	QRegExp             m_xmlElementRegex;
	QRegExp             m_xmlAttributeRegex;
	QRegExp             m_xmlValueRegex;
	QRegExp             m_xmlCommentRegex;
};

#endif // !SINXMLHIGHLIGHTER_H
