from datetime import datetime
from decimal import Decimal
from kivy.lang import Builder
from kivy.uix.textinput import TextInput
from kivy.uix.tabbedpanel import TabbedPanel, TabbedPanelItem
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.properties import BooleanProperty, ObjectProperty, NumericProperty  # pylint:disable=no-name-in-module
import re
from kivy.garden.graph import LinePlot  # pylint:disable=no-name-in-module, import-error
from math import pow, isclose

##
#   @brief              Main tabbed panel to show tabbed items in the GUI.
#
class GraphTabs(TabbedPanel):

    ##
    #   @brief          Reference to acceleration tabbed item.
    altro_tab = ObjectProperty(None)

    ##
    #   @brief          Update plots with new packet of data
    #   @param[in]      packet: new packet of data.
    def update_plot(self, s):
        self.altro_tab.update_plot_filtered(s)

    ##
    #   @brief          Update sample rate value in plots.
    #   @param[in]      instance: object calling the update function
    #   @param[in]      value: new sample rate value
    def update_sample_rate(self, instance, value):
        self.altro_tab.update_sample_rate(value)

    def update_HR_label(self, instance, value):
        self.altro_tab.update_HR_label(value)


##
#   @brief          Tabbed panel item to show acceleration data.
#
class LIS3DHTabbedPanelItem(TabbedPanelItem):


    ##
    #   @brief          Reference to graph widget.
    graph = ObjectProperty(None)

    ##
    #   @brief          Reference to plot settings widget.
    plot_settings = ObjectProperty(None)

    ##
    #   @brief          Autoscale setting.
    autoscale = BooleanProperty(False)

    def update_HR_label(self, value):
        self.plot_settings.update_HR_label(value)

    def __init__(self, **kwargs):
        self.max_seconds = 20                # Maximum number of seconds to show
        self.n_seconds = self.max_seconds    # Initial number of samples to be shown
        self.x_axis_n_points_collected = []  # Number of new collected points for x axis
        self.y_axis_n_points_collected = []  # Number of new collected points for y axis
        self.z_axis_n_points_collected = []  # Number of new collected points for z axis
        self.sample_rate = 1                 # Sample rate for data streaming
        self.sample_rate_damped = 1          # Damped sample rate for too high frequencies to plot
        super(LIS3DHTabbedPanelItem, self).__init__(**kwargs)

    ##
    #   @brief          Callback called when the graph widget is shown on the screen.
    #
    #   Here, we setup the plots for x, y, and z data.
    def on_graph(self, instance, value):
        self.graph.xmin = -self.n_seconds
        self.graph.xmax = 0
        self.graph.xlabel = 'Time (s)'
        self.graph.ylabel = 'Acceleration (g)'
        self.graph.x_ticks_minor = 1
        self.graph.x_ticks_major = 2
        self.graph.y_ticks_minor = 1
        self.graph.y_ticks_major = 1
        self.graph.x_grid_label = True
        self.graph.ymin = -2
        self.graph.ymax = 2
        self.graph.y_grid_label = True
        # Compute number of points to show
        self.n_points = self.n_seconds * self.sample_rate_damped  # Number of points to plot
        # Compute time between points on x-axis
        self.time_between_points = (self.n_seconds)/float(self.n_points)
        # Initialize x and y points list
        self.x_points = [x for x in range(-self.n_points, 0)]
        for j in range(self.n_points):
            self.x_points[j] = -self.n_seconds + \
                (j+1) * self.time_between_points
        self.x_axis_points = [0 for y in range(-self.n_points, 0)]
        self.y_axis_points = [0 for y in range(-self.n_points, 0)]

        self.x_plot = LinePlot(color=(0.75, 0.4, 0.4, 1.0))
        self.x_plot.line_width = 1.2
        self.x_plot.points = zip(self.x_points, self.x_axis_points)

        self.y_plot = LinePlot(color=(0.4, 0.4, 0.75, 1.0))
        self.y_plot.line_width = 1.2
        self.y_plot.points = zip(self.x_points, self.y_axis_points)

        self.graph.add_plot(self.x_plot)
        self.graph.add_plot(self.y_plot)

    ##
    #   @brief          Callback called when the \ref autoscale property changes.
    def on_autoscale(self, instance, value):
        if (value):
            self.autoscale_plots()

    ##
    #   @brief          Autoscale all plots.
    #
    #   Autoscale all plots in the \ref graph_widget and update y ticsk.
    def autoscale_plots(self):
        global_y_min = []
        global_y_max = []
        for plot_idx in range(3):
            if plot_idx == 0:
                yy_points = self.x_axis_points
            elif plot_idx == 1:
                yy_points = self.y_axis_points
            # Slice only the visible part
            if (abs(self.graph.xmin) < self.max_seconds):
                y_points_slice = yy_points[(
                    self.max_seconds-abs(self.graph.xmin)) * self.sample_rate_damped:]
            else:
                y_points_slice = yy_points

            global_y_min.append(min(y_points_slice))
            global_y_max.append(max(y_points_slice))

        y_min = min(global_y_min)
        y_max = max(global_y_max)

        y_max = (int(y_max * 100)+1)/100 #correct error on autoscale for y_max format
        y_min = int(y_min * 100)/100 #correct error on autoscale for y_max format
        
        if (y_min != y_max):
            min_val, max_val, major_ticks, minor_ticks = self.get_bounds_and_ticks(
                y_min, y_max, 10)
            self.graph.ymin = min_val
            self.graph.ymax = max_val
            self.graph.y_ticks_major = major_ticks
            self.graph.y_ticks_minor = minor_ticks

    def fexp(self, number):
        (sign, digits, exponent) = Decimal(number).as_tuple()
        return len(digits) + exponent - 1

    def fman(self, number):
        return float(Decimal(number).scaleb(-self.fexp(number)).normalize())

    ##
    #   @brief          Get bounds and ticks to autoscale plots.
    #
    #   @param[in]      minval: minimum value of the plot.
    #   @param[in]      maxval: maximum value of the plot.
    #   @param[in]      nticks: desired number of ticks
    def get_bounds_and_ticks(self, minval, maxval, nticks):
        # amplitude of data
        amp = maxval - minval
        # basic tick
        basictick = self.fman(amp/float(nticks))
        # correct basic tick to 1,2,5 as mantissa
        tickpower = pow(10.0, self.fexp(amp/float(nticks)))
        if basictick < 1.5:
            tick = 1.0*tickpower
            suggested_minor_tick = 4
        elif basictick >= 1.5 and basictick < 2.5:
            tick = 2.0*tickpower
            suggested_minor_tick = 4
        elif basictick >= 2.5 and basictick < 7.5:
            tick = 5.0*tickpower
            suggested_minor_tick = 5
        elif basictick >= 7.5:
            tick = 10.0*tickpower
            suggested_minor_tick = 4
        # calculate good (rounded) min and max
        goodmin = tick * (minval // tick)
        if not isclose(maxval % tick, 0.0):
            goodmax = tick * (maxval // tick + 1)
        else:
            goodmax = tick * (maxval // tick)
        return goodmin, goodmax, tick, suggested_minor_tick

    ##
    #   @brief          Callback called when \ref plot_settings widget is displayed.
    #
    #   Binding of several properties across widgets.
    def on_plot_settings(self, instance, value):
        #self.plot_settings.bind(n_seconds=self.graph.setter('xmin'))
        self.plot_settings.bind(n_seconds=self.n_seconds_updated)
        self.plot_settings.bind(ymin=self.graph.setter('ymin'))
        self.plot_settings.bind(ymax=self.graph.setter('ymax'))
        self.plot_settings.bind(autoscale_selected=self.setter('autoscale'))

    ##
    #   @brief          Number of seconds updated.
    #
    #   Update minimum value of the graph and set up x ticks.
    def n_seconds_updated(self, instance, value):
        self.graph.xmin = value
        min_val, max_val, major_ticks, minor_ticks = self.get_bounds_and_ticks(value, 0, 10)
        self.graph.x_ticks_major = major_ticks
        self.graph.x_ticks_minor = minor_ticks

    def update_plot_filtered(self, s):
        if (s.flag_first_filter):
            self.x_axis_n_points_collected, self.y_axis_n_points_collected = s.get_filtered_data()

            if self.sample_rate == 400: #plot at 200Hz
                #make sure to not lose peaks:
                for i in range(16):
                    if self.x_axis_n_points_collected[i] == 0:#no peak
                        self.x_axis_n_points_collected.pop(i)
                        self.y_axis_n_points_collected.pop(i)
                    else: #was a peak, remove the next one
                        self.x_axis_n_points_collected.pop(i+1)
                        self.y_axis_n_points_collected.pop(i+1)

            for idx in range(len(self.x_axis_n_points_collected)):
                self.x_axis_points.append(self.x_axis_points.pop(0))
                self.x_axis_points[-1] = self.x_axis_n_points_collected[idx]
                self.y_axis_points.append(self.y_axis_points.pop(0))
                self.y_axis_points[-1] = self.y_axis_n_points_collected[idx]
            self.x_plot.points = zip(self.x_points, self.x_axis_points)
            self.y_plot.points = zip(self.x_points, self.y_axis_points)
            self.x_axis_n_points_collected = []
            self.y_axis_n_points_collected = []

        if (self.autoscale):
            self.autoscale_plots()

    ##
    #   @brief          Update plots based on new sample rate value.
    #
    #   If a new sample rate is set, plots must be updated to reflect the
    #   new value of samples per second.
    def update_sample_rate(self, samples_per_second):
        self.sample_rate = samples_per_second

        if self.sample_rate == 400: #too high
            self.sample_rate_damped = 200 #1/2
        else:
            self.sample_rate_damped = self.sample_rate #no correction

        # Compute number of points to show
        self.n_points = self.n_seconds * self.sample_rate_damped  # Number of points to plot
        # Compute time between points on x-axis
        self.time_between_points = (self.n_seconds)/float(self.n_points)
        # Initialize x and y points list
        self.x_points = [x for x in range(-self.n_points, 0)]
        for j in range(self.n_points):
            self.x_points[j] = -self.n_seconds + \
                (j+1) * self.time_between_points

        self.x_axis_points = [0 for y in range(-self.n_points, 0)]
        self.y_axis_points = [0 for y in range(-self.n_points, 0)]
        self.x_plot.points = zip(self.x_points, self.x_axis_points)
        self.y_plot.points = zip(self.x_points, self.y_axis_points)


class PlotSettings(BoxLayout):
    """
    @brief Class to show some settings related to the plot.
    """

    """
    @brief Number of seconds to show on the plot.
    """
    seconds_spinner = ObjectProperty(None)

    autoscale_checkbox = ObjectProperty(None)

    """
    @brief Minimum value for y axis text input widget.
    """
    ymin_input = ObjectProperty(None)

    """
    @brief Maximum value for y axis text input widget.
    """
    ymax_input = ObjectProperty(None)

    """
    @brief Current number of seconds shown.
    """
    n_seconds = NumericProperty(0)

    autoscale_selected = BooleanProperty(False)

    ymin = NumericProperty()
    ymax = NumericProperty()

    ##
    #   @brief          Reference to label displaying bpm data
    hr_label = ObjectProperty(None)
    hr_min = ObjectProperty(None)
    hr_max = ObjectProperty(None)

    ##
    #   @brief          Update current text and display new received string.
    #
    #   @param[in]      value: the new string to be shown.
    def update_HR_label(self, value):
        self.hr_label.text = value
        for x in self.hr_min,self.hr_max:
            if x.text == '' or x.text == '-':
                x.text=value
        if value!= '-' and value != '':
            if value < self.hr_min.text:
                self.hr_min.text = value
            if value > self.hr_max.text:
                self.hr_max.text = value

    def __init__(self, **kwargs):
        super(PlotSettings, self).__init__(**kwargs)
        self.n_seconds = 20

    def on_seconds_spinner(self, instance, value):
        """
        @brief Bind change on seconds spinner to callback.
        """
        self.seconds_spinner.bind(text=self.spinner_updated)

    def on_ymin_input(self, instance, value):
        """
        @brief Bind enter pressed on ymin text input to callback.
        """
        self.ymin_input.bind(enter_pressed=self.axis_changed)

    def on_ymax_input(self, instance, value):
        """
        @brief Bind enter pressed on on ymax text input to callback.
        """
        self.ymax_input.bind(enter_pressed=self.axis_changed)

    def on_autoscale_checkbox(self, instance, value):
        self.autoscale_checkbox.bind(active=self.autoscale_changed)

    def autoscale_changed(self, instance, value):
        self.ymin_input.disabled = value
        self.ymax_input.disabled = value
        self.autoscale_selected = value

    def spinner_updated(self, instance, value):
        """
        @brief Get new value of seconds to show on the plot.
        """
        self.n_seconds = -int(self.seconds_spinner.text)

    def axis_changed(self, instance, focused):
        """
        @brief Called when a new value of ymin or ymax is entered on the GUI.
        """
        if (not focused):
            if (not ((self.ymin_input.text == '') or (self.ymax_input.text == ''))):
                y_min = float(self.ymin_input.text)
                y_max = float(self.ymax_input.text)
                if (y_min >= y_max):
                    self.ymin_input.text = f"{self.ymin:.2f}"
                    self.ymax_input.text = f"{self.ymax:.2f}"
                else:
                    self.ymin = y_min
                    self.ymax = y_max
            elif (self.ymin_input.text == ''):
                self.ymin_input.text = f"{self.ymin:.2f}"
            elif (self.ymax_input.text == ''):
                self.ymax_input.text = f"{self.ymax:.2f}"


class FloatInput(TextInput):
    pat = re.compile('[^0-9]')
    enter_pressed = BooleanProperty(None)

    def __init__(self, **kwargs):
        super(FloatInput, self).__init__(**kwargs)
        self.bind(focus=self.on_focus)  # pylint:disable=no-member
        self.multiline = False

    def insert_text(self, substring, from_undo=False):
        pat = self.pat
        if ((len(self.text) == 0) and substring == '-'):
            s = '-'
        else:
            if '.' in self.text:
                s = re.sub(pat, '', substring)
            else:
                s = '.'.join([re.sub(pat, '', s)
                             for s in substring.split('.', 1)])
        return super(FloatInput, self).insert_text(s, from_undo=from_undo)

    def on_focus(self, instance, value):
        self.enter_pressed = value
