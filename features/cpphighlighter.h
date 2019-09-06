  #ifndef HIGHLIGHTER_H
  #define HIGHLIGHTER_H

  #include <QSyntaxHighlighter>
  #include <QTextCharFormat>

  class QTextDocument;

  class CPPHighLighter : public QSyntaxHighlighter
  {
      Q_OBJECT

  public:
      CPPHighLighter(QTextDocument *parent = 0);

  protected:
      void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

  private:
      struct HighlightingRule
      {
          QRegExp pattern;
          QTextCharFormat format;
      };
      QVector<HighlightingRule> highlightingRules;

      QRegExp commentStartExpression;
      QRegExp commentEndExpression;

      QTextCharFormat keywordFormat;
      QTextCharFormat classFormat;
      QTextCharFormat singleLineCommentFormat;
      QTextCharFormat multiLineCommentFormat;
      QTextCharFormat doubleQuotationFormat, singleQuotationFormat;
      QTextCharFormat functionFormat;
  };

  #endif // HIGHLIGHTER_H
