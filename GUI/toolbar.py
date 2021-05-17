from kivy.properties import NumericProperty, ObjectProperty, StringProperty
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.popup import Popup
from communication import KivySerial

class Toolbar(BoxLayout):
    """
    @brief Lateral toolbar widget.

    The toolbar widget is used to show some
    settings related to the app.
    """

    """
    @brief String used to send messages to bottom bar.
    """
    message_string = StringProperty("")

    def __init__(self, **kwargs):
        super(Toolbar, self).__init__(**kwargs)

    def sample_rate_dialog(self):
        """
        @brief Open popup for wave selection.
        """
        self.message_string = "Sample Rate Dialog"
        popup = SampleRateDialog()
        popup.open()

    def full_scale_range_dialog(self):
        """
        @brief Open popup for wave selection.
        """
        self.message_string = "Range Selection Dialog"
        popup = FullScaleRangeDialog()
        popup.open()



class SampleRateDialog(Popup):
    """
    @brief Popup to allow wave selection 
    """
    sample_rate_spinner = ObjectProperty(None)

    def __init__(self, **kwargs):
        self.board = KivySerial()
        super(SampleRateDialog, self).__init__(**kwargs)

    def on_sample_rate_spinner(self, instance, value):
        self.sample_rate_spinner.text = f'{self.board.sample_rate} Hz'

    ##
    #   @brief          Callback called when update button is pressed.
    #
    #   If the board is connected, update the sample rate.
    #
    def update_pressed(self):
        
        if (self.board.is_connected()):
           self.board.update_sample_rate_on_board(self.sample_rate_spinner.text)
        self.dismiss()


class FullScaleRangeDialog(Popup):
    """
    @brief Popup to allow range selection 
    """
    fsr_select_spinner = ObjectProperty(None)

    def __init__(self, **kwargs):
        super(FullScaleRangeDialog, self).__init__(**kwargs)
        self.board = KivySerial()

    def on_sample_rate_spinner(self, instance, value):
        self.fsr_select_spinner.text = f'{self.board.full_scale_range}'

    ##
    #   @brief          Callback called when update button is pressed.
    #
    #   If the board is connected, update the full scale range.
    #
    def update_pressed(self):
        """
        @brief Callback called when update button is pressed.

        If the board is connected, update the range selection.
        """
        if (self.board.is_connected()):
            self.board.update_fsr_on_board(self.fsr_select_spinner.text)
        self.dismiss()