#include "HelpDialog.h"
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QDialogButtonBox>

HelpDialog::HelpDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Help"));
    resize(550, 350); // компактный размер, но достаточно для чтения

    QVBoxLayout *layout = new QVBoxLayout(this);

    QTextBrowser *textBrowser = new QTextBrowser(this);
    textBrowser->setReadOnly(true);
    textBrowser->setOpenExternalLinks(false);

    QString helpText =
        "<h2>Fraction Calculator Help</h2>"
        "<p>This application allows you to perform arithmetic operations with fractions.</p>"

        "<h3>Input Formats</h3>"
        "<ul>"
        "<li><b>Integer:</b> just type the number (e.g., 5)</li>"
        "<li><b>Simple fraction:</b> numerator / denominator (e.g., 3/4)</li>"
        "<li><b>Mixed number:</b> whole|numerator/denominator (e.g., 2|1/3)</li>"
        "<li>You can change sign with the <b>+/-</b> button at any time.</li>"
        "</ul>"

        "<h3>Basic Operations</h3>"
        "<ul>"
        "<li><b>+</b> Addition</li>"
        "<li><b>-</b> Subtraction</li>"
        "<li><b>*</b> Multiplication</li>"
        "<li><b>/</b> Division (also used as fraction separator during input)</li>"
        "</ul>"

        "<h3>Functions</h3>"
        "<ul>"
        "<li><b>x²</b> – Square of the current number</li>"
        "<li><b>1/x</b> – Reciprocal (inverse) of the current number</li>"
        "<li><b>+/-</b> – Change sign (negate)</li>"
        "</ul>"

        "<h3>Memory</h3>"
        "<ul>"
        "<li><b>MS</b> – Store current value into memory</li>"
        "<li><b>MR</b> – Recall value from memory</li>"
        "<li><b>MC</b> – Clear memory</li>"
        "<li><b>M+</b> – Add current value to memory</li>"
        "<li>Memory state is shown in the status line as <i>_On</i> (value present) or <i>_Off</i> (empty).</li>"
        "</ul>"

        "<h3>Clipboard</h3>"
        "<ul>"
        "<li><b>Copy</b> – Copy current display to clipboard (as text)</li>"
        "<li><b>Paste</b> – Paste a fraction from clipboard (must be in valid format)</li>"
        "</ul>"

        "<h3>Editing</h3>"
        "<ul>"
        "<li><b>←</b> – Backspace: delete the last character</li>"
        "<li><b>CE</b> – Clear current entry (reset to 0/1)</li>"
        "<li><b>C</b> – Clear all (reset calculator)</li>"
        "<li><b>|</b> – Insert whole-part separator for mixed numbers</li>"
        "</ul>"

        "<h3>History</h3>"
        "<p>The left panel shows the history of completed calculations. "
        "Each entry shows the full expression and its result. The history "
        "automatically scrolls to the latest entry.</p>"

        "<h3>Settings</h3>"
        "<p>Use the <b>Settings</b> menu to:</p>"
        "<ul>"
        "<li>Choose display format: always as fraction, or as decimal if the result is a terminating decimal.</li>"
        "<li>Select operand source for operations (memory or clipboard).</li>"
        "<li>Adjust extra options: auto‑clear after equals, history size, sound on/off.</li>"
        "<li>Save or load your preferences to/from a file.</li>"
        "</ul>"

        "<h3>Examples</h3>"
        "<ul>"
        "<li>Enter <code>2|1/3</code> (press 2, |, 1, /, 3) – shows <code>2|1/3</code>.</li>"
        "<li>Press <code>+</code>, then enter <code>7/9</code>, press <code>=</code> – result: <code>3|1/9</code> (or decimal).</li>"
        "<li>Press <code>x²</code> on a number to square it.</li>"
        "<li>Use memory: store a value with <b>MS</b>, clear display with <b>C</b>, recall with <b>MR</b>.</li>"
        "</ul>"

        "<p><i>For more information, see the documentation.</i></p>";

    textBrowser->setHtml(helpText);
    layout->addWidget(textBrowser);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}
