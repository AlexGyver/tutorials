from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice

# https://stackoverflow.com/questions/35932660/qcombobox-click-event обновление по клику

app = QtWidgets.QApplication([])
ui = uic.loadUi("design.ui")
ui.setWindowTitle("SerialGUI")

serial = QSerialPort()
serial.setBaudRate(115200)


# прочитать порт
def onRead():
    rx = serial.readLine()
    data = str(rx, 'utf-8').strip().split(',')  # подрезать и разбить по запятым

    # парсинг
    if data[0] == '0':
        print(data[1])

    if data[0] == '1':
        print(data[1])

    if data[0] == '2':
        print(data[1])


# открыть порт
def onOpen():
    serial.setPortName(ui.comL.currentText())
    serial.open(QIODevice.ReadWrite)


# отправить массив int данных
def serialSend(data):
    # короткий способ перевести int data в строки, соединить через запятую и прибавить \n
    txs = ','.join(map(str, data)) + '\n'
    serial.write(txs.encode())  # преобразовать в байты и отправить


# закрыть порт
def onClose():
    serial.close()

# обновить список портов
def updateList():
    portList = []
    ports = QSerialPortInfo().availablePorts()
    for port in ports: portList.append(port.portName())
    ui.comL.clear()
    ui.comL.addItems(portList)


# пошла программа
serial.readyRead.connect(onRead)
ui.openB.clicked.connect(onOpen)
ui.closeB.clicked.connect(onClose)
updateList()

ui.show()
app.exec()