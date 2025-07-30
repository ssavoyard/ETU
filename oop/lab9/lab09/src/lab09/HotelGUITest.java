package lab09;

import org.junit.*;

import javax.swing.table.DefaultTableModel;

public class HotelGUITest {

    @Test
    public void testAddGuest() {
        GUI gui = new GUI();
        gui.buildAndShowGUI();

        DefaultTableModel model = gui.tableModel;
        int initialRowCount = model.getRowCount();

        model.addRow(new Object[]{"Alice Brown", "305", "12.12.2024", "20.12.2024"});

        Assert.assertEquals(initialRowCount + 1, model.getRowCount());

        Object[] lastRow = {
            model.getValueAt(initialRowCount, 0),
            model.getValueAt(initialRowCount, 1),
            model.getValueAt(initialRowCount, 2),
            model.getValueAt(initialRowCount, 3)
        };
        Assert.assertArrayEquals(new Object[]{"Alice Brown", "305", "12.12.2024", "20.12.2024"}, lastRow);
    }

    @Test
    public void testSortByName() {
        GUI gui = new GUI();
        gui.buildAndShowGUI();

        DefaultTableModel model = gui.tableModel;

        // Проверяем начальное состояние таблицы
        String firstGuestNameBeforeSort = (String) model.getValueAt(0, 0);
        String secondGuestNameBeforeSort = (String) model.getValueAt(1, 0);

        // Сортируем таблицу по имени
        gui.sortTable(0);

        // Проверяем, что таблица отсортирована
        String firstGuestNameAfterSort = (String) model.getValueAt(0, 0);
        String secondGuestNameAfterSort = (String) model.getValueAt(1, 0);

        Assert.assertTrue(firstGuestNameAfterSort.compareTo(secondGuestNameAfterSort) <= 0);

        Assert.assertNotEquals(firstGuestNameBeforeSort, firstGuestNameAfterSort);
    }
    
    @Test(expected = RuntimeException.class)
    public void testException() {
    throw new RuntimeException("Error encountered");
    }
    @BeforeClass
     public static void allTestsStarted() {
     System.out.println("Testing started");
     }
     @AfterClass
     public static void allTestsFinished() {
     System.out.println("Testing finished");
     }
     @Before
     public void testStarted() {
     System.out.println("Test started");
     }
     @After
     public void testFinished() {
     System.out.println("Test finished");
     }
}
