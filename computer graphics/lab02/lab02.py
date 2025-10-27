import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Button


class BezierCurve:
    def __init__(self):
        self.fig, self.ax = plt.subplots(figsize=(10, 8))
        plt.subplots_adjust(bottom=0.2)

        self.control_points = []
        self.dragging_point = None
        self.dragging = False

        # Графические элементы для быстрой перерисовки
        self.points_line, = self.ax.plot([], [], 'ro-', markersize=8, linewidth=1)
        self.curve_line, = self.ax.plot([], [], 'b-', linewidth=2)

        self.setup_plot()
        self.setup_ui()
        self.redraw()

    def setup_plot(self):
        self.ax.set_xlim(0, 10)
        self.ax.set_ylim(0, 10)
        self.ax.grid(True, alpha=0.3)
        self.ax.set_title('Кривая Безье - Кликните чтобы добавить точки')
        self.ax.set_aspect('equal')

        self.fig.canvas.mpl_connect('button_press_event', self.on_click)
        self.fig.canvas.mpl_connect('button_release_event', self.on_release)
        self.fig.canvas.mpl_connect('motion_notify_event', self.on_motion)

    def setup_ui(self):
        ax_add = plt.axes([0.1, 0.05, 0.15, 0.075])
        ax_clear = plt.axes([0.3, 0.05, 0.15, 0.075])
        ax_reset = plt.axes([0.5, 0.05, 0.15, 0.075])

        self.btn_add = Button(ax_add, 'Добавить точку')
        self.btn_clear = Button(ax_clear, 'Очистить')
        self.btn_reset = Button(ax_reset, 'Сброс')

        self.btn_add.on_clicked(self.add_random_point)
        self.btn_clear.on_clicked(self.clear_points)
        self.btn_reset.on_clicked(self.reset_view)

    def bezier_curve(self, points, num_points=100):
        #cтроит кривую Безье через заданные точки
        if len(points) < 2:
            return np.array([])

        n = len(points) - 1
        t = np.linspace(0, 1, num_points)

        curve = np.zeros((num_points, 2))
        for i in range(num_points):
            curve[i] = self.de_casteljau(points, t[i])

        return curve

    def de_casteljau(self, points, t):
        #алгоритм де Кастильо
        if len(points) == 1:
            return points[0]

        new_points = []
        for i in range(len(points) - 1):
            x = (1 - t) * points[i][0] + t * points[i + 1][0]
            y = (1 - t) * points[i][1] + t * points[i + 1][1]
            new_points.append([x, y])

        return self.de_casteljau(new_points, t)

    def on_click(self, event):

        if event.inaxes != self.ax:
            return

        if event.button == 1:  #левая кнопка мыши
            #проверяем кликнули ли на существующую точку
            for i, point in enumerate(self.control_points):
                distance = np.sqrt((event.xdata - point[0]) ** 2 + (event.ydata - point[1]) ** 2)
                if distance < 0.3:
                    self.dragging_point = i
                    self.dragging = True
                    return

            #добавляем новую точку
            self.control_points.append([event.xdata, event.ydata])
            self.redraw()

        elif event.button == 3:  #правая кнопка мыши
            for i, point in enumerate(self.control_points):
                distance = np.sqrt((event.xdata - point[0]) ** 2 + (event.ydata - point[1]) ** 2)
                if distance < 0.3:
                    self.control_points.pop(i)
                    self.redraw()
                    break

    def on_release(self, event):

        self.dragging_point = None
        self.dragging = False

    def on_motion(self, event):
        #обработчик движения мыши (обновление данных массива точек)
        if self.dragging_point is None or event.inaxes != self.ax or not self.dragging:
            return

        #обновление координаты точки
        self.control_points[self.dragging_point] = [event.xdata, event.ydata]

        self.update_plot_data()

    def update_plot_data(self):

        if len(self.control_points) > 0:
            points_array = np.array(self.control_points)
            self.points_line.set_data(points_array[:, 0], points_array[:, 1])

            if len(self.control_points) >= 2:
                curve = self.bezier_curve(self.control_points)
                self.curve_line.set_data(curve[:, 0], curve[:, 1])
            else:
                self.curve_line.set_data([], [])
        else:
            self.points_line.set_data([], [])
            self.curve_line.set_data([], [])

        #только обновляем canvas без полной перерисовки
        self.fig.canvas.draw_idle()

    def add_random_point(self, event):
        x = np.random.uniform(1, 9)
        y = np.random.uniform(1, 9)
        self.control_points.append([x, y])
        self.redraw()

    def clear_points(self, event):
        self.control_points = []
        self.redraw()

    def reset_view(self, event):
        self.ax.set_xlim(0, 10)
        self.ax.set_ylim(0, 10)
        self.redraw()

    def redraw(self):
        #полная перерисовка (когда добавление или удаление точек)
        self.ax.clear()
        self.ax.set_xlim(0, 10)
        self.ax.set_ylim(0, 10)
        self.ax.grid(True, alpha=0.3)
        self.ax.set_title(f'Кривая Безье - {len(self.control_points)} точек')
        self.ax.set_aspect('equal')

        #пересоздание графических элементов
        self.points_line, = self.ax.plot([], [], 'ro-', markersize=8, linewidth=1, label='Контрольные точки')
        self.curve_line, = self.ax.plot([], [], 'b-', linewidth=2, label='Кривая Безье')

        if len(self.control_points) > 0:
            self.update_plot_data()
            self.ax.legend()

        self.fig.canvas.draw()

if __name__ == "__main__":
    app = BezierCurve()
    plt.show()