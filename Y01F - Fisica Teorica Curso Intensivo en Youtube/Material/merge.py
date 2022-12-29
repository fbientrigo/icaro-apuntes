n = []
name = 'FT1_clase'

from PyPDF2 import PdfFileMerger

for i in range(1,27):
    if i < 10:
        i = str('0') + str(i)
    else:
        i = str(i)
    n.append(name + i + '.pdf')

merger = PdfFileMerger()

for pdf in n:
    merger.append(pdf)

merger.write('resultado.pdf')
merger.close()
