import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Button, RadioButtons, Slider, TextBox


class GraphicsLab:
    def __init__(self):
        self.fig, self.ax = plt.subplots(figsize=(10, 8))
        plt.subplots_adjust(left=0.1, right=0.75, bottom=0.3, top=0.95)

        # доступные объекты
        self.objects = {
            'Треугольник': np.array([[0.2, 0.2], [0.5, 0.8], [0.8, 0.2], [0.2, 0.2]]),
            'Квадрат': np.array([[0.2, 0.2], [0.2, 0.8], [0.8, 0.8], [0.8, 0.2], [0.2, 0.2]]),
            'Прямоугольник': np.array([[0.1, 0.2], [0.1, 0.8], [0.9, 0.8], [0.9, 0.2], [0.1, 0.2]]),
            'Пятиугольник': self.create_polygon(5, 0.5, 0.5, 0.3),
        }

        self.current_object = 'Треугольник'
        self.rotation_angle = 0  # градусы - поворот относительно центра
        self.origin_rotation_angle = 0  # градусы - поворот относительно начала координат

        self.setup_ui()
        self.draw_initial()

    def create_polygon(self, n, cx, cy, radius):
        angles = np.linspace(0, 2 * np.pi, n + 1)
        x = cx + radius * np.cos(angles)
        y = cy + radius * np.sin(angles)
        return np.column_stack([x, y])

    def create_circle(self, cx, cy, radius, points=100):
        angles = np.linspace(0, 2 * np.pi, points)
        x = cx + radius * np.cos(angles)
        y = cy + radius * np.sin(angles)
        return np.column_stack([x, y])

    def create_oval(self, cx, cy, rx, ry, points=100):
        angles = np.linspace(0, 2 * np.pi, points)
        x = cx + rx * np.cos(angles)
        y = cy + ry * np.sin(angles)
        return np.column_stack([x, y])

    def setup_ui(self):
        # настройка интерфейса
        # кнопки для выбора фигуры
        ax_radio = plt.axes([0.77, 0.5, 0.2, 0.4])
        self.radio = RadioButtons(ax_radio, list(self.objects.keys()) + ['Круг', 'Овал'])
        self.radio.on_clicked(self.on_object_change)

        # слайдер для поворота относительно центра
        ax_slider = plt.axes([0.15, 0.2, 0.5, 0.03])
        self.slider = Slider(ax_slider, 'Поворот (°)', -180, 180, valinit=0, valfmt=None)
        self.slider.on_changed(self.on_rotation_change)

        # текстовое поле для ввода угла относительно центра
        ax_angle_input = plt.axes([0.66, 0.2, 0.08, 0.03])
        self.angle_box = TextBox(ax_angle_input, '', initial='0')
        self.angle_box.on_submit(self.on_angle_submit)

        #слайдер для поворота относительно начала координат
        ax_origin_slider = plt.axes([0.15, 0.15, 0.5, 0.03])
        self.origin_slider = Slider(ax_origin_slider, 'Поворот отн. (0,0) (°)', -180, 180, valinit=0, valfmt=None)
        self.origin_slider.on_changed(self.on_origin_rotation_change)

        #текстовое поле для ввода угла относительно начала координат
        ax_origin_angle_input = plt.axes([0.66, 0.15, 0.08, 0.03])
        self.origin_angle_box = TextBox(ax_origin_angle_input, '', initial='0')
        self.origin_angle_box.on_submit(self.on_origin_angle_submit)

        #кнопки преобразований
        button_width = 0.15
        button_height = 0.06

        # отразить по Х, У, масштаб и искажение, ресет
        ax_reflect_x = plt.axes([0.1, 0.08, button_width, button_height])
        ax_reflect_y = plt.axes([0.3, 0.08, button_width, button_height])
        ax_scale_u = plt.axes([0.5, 0.08, button_width, button_height])
        ax_scale_nu = plt.axes([0.7, 0.08, button_width, button_height])
        ax_reset = plt.axes([0.3, 0.01, button_width, button_height])

        self.btn_reflect_x = Button(ax_reflect_x, 'Отразить X')
        self.btn_reflect_y = Button(ax_reflect_y, 'Отразить Y')
        self.btn_scale_u = Button(ax_scale_u, 'Увеличить')
        self.btn_scale_nu = Button(ax_scale_nu, 'Исказить')
        self.btn_reset = Button(ax_reset, 'Сброс')

        # обработчики
        self.btn_reflect_x.on_clicked(self.on_reflect_x)
        self.btn_reflect_y.on_clicked(self.on_reflect_y)
        self.btn_scale_u.on_clicked(self.on_scale_uniform)
        self.btn_scale_nu.on_clicked(self.on_scale_non_uniform)
        self.btn_reset.on_clicked(self.on_reset)

    def setup_axes(self):
        self.ax.clear()
        self.ax.set_xlim(-2, 2)
        self.ax.set_ylim(-2, 2)

        # оси
        self.ax.axhline(0, color='black', linewidth=0.5, alpha=0.7)
        self.ax.axvline(0, color='black', linewidth=0.5, alpha=0.7)

        # сетка
        self.ax.grid(True, which='both', color='gray', linestyle='-', linewidth=0.3, alpha=0.3)
        self.ax.set_xticks(np.arange(-2, 2.1, 0.2))
        self.ax.set_yticks(np.arange(-2, 2.1, 0.2))

        # деления
        self.ax.set_xticks(np.arange(-2, 2.1, 1), minor=False)
        self.ax.set_yticks(np.arange(-2, 2.1, 1), minor=False)

    def get_current_object(self):
        # возвращает текущий объект
        if self.current_object == 'Круг':
            return self.create_circle(0.5, 0.5, 0.3)
        elif self.current_object == 'Овал':
            return self.create_oval(0.5, 0.5, 0.4, 0.2)
        else:
            return self.objects.get(self.current_object, self.objects['Треугольник']).copy()

    def apply_rotation(self, points):
        # применяет поворот к точкам относительно центра
        if self.rotation_angle == 0:
            return points

        angle_rad = np.radians(self.rotation_angle)
        cos_a, sin_a = np.cos(angle_rad), np.sin(angle_rad)

        # центр фигуры
        center = np.mean(points, axis=0)

        # поворот относительно центра
        rotated = points.copy()
        rotated -= center
        rotated = np.dot(rotated, np.array([[cos_a, -sin_a], [sin_a, cos_a]]))
        rotated += center

        return rotated

    def apply_origin_rotation(self, points):
        # применяет поворот к точкам относительно начала координат (0,0)
        if self.origin_rotation_angle == 0:
            return points

        angle_rad = np.radians(self.origin_rotation_angle)
        cos_a, sin_a = np.cos(angle_rad), np.sin(angle_rad)

        # поворот относительно начала координат
        rotated = points.copy()
        rotated = np.dot(rotated, np.array([[cos_a, -sin_a], [sin_a, cos_a]]))

        return rotated

    def apply_all_rotations(self, points):
        # применяет оба поворота: сначала относительно начала координат, потом относительно центра
        points = self.apply_origin_rotation(points)
        points = self.apply_rotation(points)
        return points

    def apply_transform(self, points, transform_matrix):
        # применяет аффинное преобразование
        homogeneous_points = np.hstack([points, np.ones((points.shape[0], 1))])
        transformed_points = homogeneous_points @ transform_matrix.T
        return transformed_points[:, :2]

    def draw_initial(self):
        # первоначальная отрисовка
        self.setup_axes()

        original = self.get_current_object()
        rotated = self.apply_all_rotations(original)

        self.ax.plot(rotated[:, 0], rotated[:, 1], 'b-', linewidth=2, label='Исходный объект')
        self.ax.legend()
        self.ax.set_aspect('equal')
        self.fig.canvas.draw()

    def redraw(self, transformed=None, label=''):
        # перерисовывает график
        self.setup_axes()

        original = self.get_current_object()
        rotated_original = self.apply_all_rotations(original)

        self.ax.plot(rotated_original[:, 0], rotated_original[:, 1], 'b-', linewidth=2, label='Исходный объект')

        if transformed is not None:
            rotated_transformed = self.apply_all_rotations(transformed)
            self.ax.plot(rotated_transformed[:, 0], rotated_transformed[:, 1], 'r-', linewidth=2, label=label)

        self.ax.legend()
        self.ax.set_aspect('equal')
        self.fig.canvas.draw()

    def on_object_change(self, label):
        # обработчик смены объекта
        self.current_object = label
        self.redraw()

    def on_rotation_change(self, val):
        # обработчик изменения поворота относительно центра
        self.rotation_angle = val
        self.angle_box.set_val(str(int(val)))
        self.redraw()

    def on_angle_submit(self, text):
        # обработчик ввода угла относительно центра
        try:
            angle = float(text)
            angle = max(-180, min(180, angle))
            self.rotation_angle = angle
            self.slider.set_val(angle)
            self.redraw()
        except ValueError:
            self.angle_box.set_val(str(int(self.rotation_angle)))

    def on_origin_rotation_change(self, val):
        # обработчик изменения поворота относительно начала координат
        self.origin_rotation_angle = val
        self.origin_angle_box.set_val(str(int(val)))
        self.redraw()

    def on_origin_angle_submit(self, text):
        # обработчик ввода угла относительно начала координат
        try:
            angle = float(text)
            angle = max(-180, min(180, angle))
            self.origin_rotation_angle = angle
            self.origin_slider.set_val(angle)
            self.redraw()
        except ValueError:
            self.origin_angle_box.set_val(str(int(self.origin_rotation_angle)))

    def on_reflect_x(self, event):
        original = self.get_current_object()
        rotated_original = self.apply_all_rotations(original)  # повернутая исходная фигура

        transform = np.array([[1, 0, 0], [0, -1, 0], [0, 0, 1]])
        transformed = self.apply_transform(rotated_original, transform)  # отражение повернутой

        self.redraw_transformed(transformed, 'Отражение по X')

    def on_reflect_y(self, event):
        original = self.get_current_object()
        rotated_original = self.apply_all_rotations(original)

        transform = np.array([[-1, 0, 0], [0, 1, 0], [0, 0, 1]])
        transformed = self.apply_transform(rotated_original, transform)

        self.redraw_transformed(transformed, 'Отражение по Y')

    def redraw_transformed(self, transformed, label):
        # перерисовывает график для преобразованной фигуры
        self.setup_axes()

        # исходная фигура повернутая
        original = self.get_current_object()
        rotated_original = self.apply_all_rotations(original)
        self.ax.plot(rotated_original[:, 0], rotated_original[:, 1], 'b-', linewidth=2, label='Исходный объект')

        # преобразованная фигур отраженная
        self.ax.plot(transformed[:, 0], transformed[:, 1], 'r-', linewidth=2, label=label)

        self.ax.legend()
        self.ax.set_aspect('equal')
        self.fig.canvas.draw()

    def on_scale_uniform(self, event):
        # равномерное масштабирование
        original = self.get_current_object()
        transform = np.array([[1.5, 0, 0], [0, 1.5, 0], [0, 0, 1]])
        transformed = self.apply_transform(original, transform)
        self.redraw(transformed, 'Равномерное масштабирование')

    def on_scale_non_uniform(self, event):
        # неравномерное масштабирование
        original = self.get_current_object()
        transform = np.array([[1.52, 0, 0], [0, -2.28, 0], [0, 0, 1]])
        transformed = self.apply_transform(original, transform)
        self.redraw(transformed, 'Неравномерное масштабирование')

    def on_reset(self, event):
        # сброс к исходному состоянию
        self.rotation_angle = 0
        self.origin_rotation_angle = 0
        self.slider.set_val(0)
        self.angle_box.set_val('0')
        self.origin_slider.set_val(0)
        self.origin_angle_box.set_val('0')
        self.redraw()


# запуск приложения
if __name__ == "__main__":
    lab = GraphicsLab()
    plt.show()