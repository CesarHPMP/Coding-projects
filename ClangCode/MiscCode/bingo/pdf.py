import pdfkit

# Correct path to wkhtmltopdf
path_to_wkhtmltopdf = r'/usr/bin/wkhtmltopdf'

config = pdfkit.configuration(wkhtmltopdf=path_to_wkhtmltopdf)

options = {
    'page-size': 'A4',
    'copies': 1,
    'margin-top': '0mm',
    'margin-bottom': '0mm',
    'margin-left': '0mm',
    'margin-right': '0mm',
    'enable-local-file-access': None,  # Enable access to local files
    'zoom': 100
}

pdfkit.from_file('folha_1.html', 'folha_1.pdf', configuration=config, options=options)

