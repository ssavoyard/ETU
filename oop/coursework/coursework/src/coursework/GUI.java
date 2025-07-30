package coursework;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class GUI {
    private JFrame frame;
    private JTabbedPane tabbedPane;
    private List<Room> rooms = new ArrayList<>();
    private List<Guest> guests = new ArrayList<>();
    private List<Employee> employees = new ArrayList<>();
    private List<Price> prices = new ArrayList<>();

    private JTable roomTable;
    private JTable guestTable;
    private JTable employeeTable;
    private JTable priceTable;

    private DefaultTableModel roomTableModel;
    private DefaultTableModel guestTableModel;
    private DefaultTableModel employeeTableModel;
    private DefaultTableModel priceTableModel;
    private JLabel guestCountLabel;
    private JLabel earningsLabel;

    public void buildAndShowGUI() {
        frame = new JFrame("Hotel Management System");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 600);

        // Создание меню
        createMenuBar();
        createButtonPanel();

        // Создание вкладок
        tabbedPane = new JTabbedPane();
        tabbedPane.addTab("Rooms", createRoomPanel());
        tabbedPane.addTab("Guests", createGuestPanel());
        tabbedPane.addTab("Employees", createEmployeePanel());
        tabbedPane.addTab("Prices", createPricePanel());
        tabbedPane.addTab("Statistics", createStatisticsPanel());

        frame.add(tabbedPane, BorderLayout.CENTER);
        frame.setVisible(true);
    }

    private void createMenuBar() {
        JMenuBar menuBar = new JMenuBar();

     // File menu
        JMenu fileMenu = new JMenu("File");
        JMenuItem loadMenuItem = new JMenuItem("Load");
        loadMenuItem.addActionListener(e -> {
            try {
                FileHandler.loadData(frame, roomTableModel, guestTableModel, employeeTableModel, priceTableModel);
                JOptionPane.showMessageDialog(frame, "Data loaded successfully!");
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(frame, "Error loading data: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
            }
        });
        fileMenu.add(loadMenuItem);

        JMenuItem saveMenuItem = new JMenuItem("Save");
        saveMenuItem.addActionListener(e -> {
            try {
                FileHandler.saveData(frame, roomTableModel, guestTableModel, employeeTableModel, priceTableModel);
                JOptionPane.showMessageDialog(frame, "Data saved successfully!");
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(frame, "Error saving data: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
            }
        });
        fileMenu.add(saveMenuItem);

        menuBar.add(fileMenu);

        JMenu editMenu = new JMenu("Edit");
        JMenuItem sortItem = new JMenuItem("Sort");
        sortItem.addActionListener(e -> sortCurrentTab());
        editMenu.add(sortItem);

        JMenuItem findItem = new JMenuItem("Find");
        findItem.addActionListener(e -> findInCurrentTab());
        editMenu.add(findItem);

        JMenu exportMenu = new JMenu("Export");
        exportMenu.add(new JMenuItem("Export as PDF"));
        exportMenu.add(new JMenuItem("Export as HTML"));

        menuBar.add(fileMenu);
        menuBar.add(editMenu);
        menuBar.add(exportMenu);

        frame.setJMenuBar(menuBar);
    }

    private void createButtonPanel() {
    	JPanel buttonPanel = new JPanel();
        JButton addButton = new JButton("Add");
        addButton.addActionListener(e -> addRow());
        JButton deleteButton = new JButton("Delete");
        deleteButton.addActionListener(e -> deleteRow());
        buttonPanel.add(addButton);
        buttonPanel.add(deleteButton);

        frame.add(buttonPanel, BorderLayout.SOUTH);
    }
    
    private JPanel createRoomPanel() {
        JPanel panel = new JPanel(new BorderLayout());

        String[] columnNames = {"Room Number", "Room Type", "Occupied By"};
        roomTableModel = new DefaultTableModel(columnNames, 0);
        roomTable = new JTable(roomTableModel);

        JScrollPane scrollPane = new JScrollPane(roomTable);
        panel.add(scrollPane, BorderLayout.CENTER);
        return panel;
    }

    private JPanel createGuestPanel() {
        JPanel panel = new JPanel(new BorderLayout());

        String[] columnNames = {"Name", "Room Number", "Stay Duration (days)"};
        guestTableModel = new DefaultTableModel(columnNames, 0);
        guestTable = new JTable(guestTableModel);

        JScrollPane scrollPane = new JScrollPane(guestTable);
        panel.add(scrollPane, BorderLayout.CENTER);
        return panel;
    }

    private JPanel createEmployeePanel() {
        JPanel panel = new JPanel(new BorderLayout());

        String[] columnNames = {"Name", "Position", "Room Type"};
        employeeTableModel = new DefaultTableModel(columnNames, 0);
        employeeTable = new JTable(employeeTableModel);

        JScrollPane scrollPane = new JScrollPane(employeeTable);
        panel.add(scrollPane, BorderLayout.CENTER);
        return panel;
    }

    private JPanel createPricePanel() {
        JPanel panel = new JPanel(new BorderLayout());

        String[] columnNames = {"Service Type", "Price"};
        priceTableModel = new DefaultTableModel(columnNames, 0);
        priceTable = new JTable(priceTableModel);

        JScrollPane scrollPane = new JScrollPane(priceTable);
        panel.add(scrollPane, BorderLayout.CENTER);
        return panel;
    }


    private JPanel createStatisticsPanel() {
        JPanel panel = new JPanel(new GridLayout(2, 1));
        guestCountLabel = new JLabel("Total Guests This Month: 0");
        earningsLabel = new JLabel("Total Earnings: $0.00");

        panel.add(guestCountLabel);
        panel.add(earningsLabel);
        return panel;
    }

    private void sortCurrentTab() {
        int selectedTab = tabbedPane.getSelectedIndex();
        DefaultTableModel model = getCurrentTableModel(selectedTab);

        if (model != null) {
            List<Object[]> rows = new ArrayList<>();
            for (int i = 0; i < model.getRowCount(); i++) {
                Object[] row = new Object[model.getColumnCount()];
                for (int j = 0; j < model.getColumnCount(); j++) {
                    row[j] = model.getValueAt(i, j);
                }
                rows.add(row);
            }

            rows.sort(Comparator.comparing(o -> o[0].toString()));

            model.setRowCount(0);
            for (Object[] row : rows) {
                model.addRow(row);
            }
        }
    }

    private void findInCurrentTab() {
        String searchTerm = JOptionPane.showInputDialog(frame, "Enter search term:");
        if (searchTerm == null || searchTerm.isEmpty()) {
            return;
        }

        int selectedTab = tabbedPane.getSelectedIndex();
        DefaultTableModel model = getCurrentTableModel(selectedTab);

        if (model != null) {
            for (int i = 0; i < model.getRowCount(); i++) {
                String value = model.getValueAt(i, 0).toString();
                if (value.contains(searchTerm)) {
                    JOptionPane.showMessageDialog(frame, "Found: " + value);
                    return;
                }
            }
            JOptionPane.showMessageDialog(frame, "No match found.");
        }
    }

    private DefaultTableModel getCurrentTableModel(int selectedTab) {
        switch (selectedTab) {
            case 0: return roomTableModel;
            case 1: return guestTableModel;
            default: return null; // No sort or find functionality for other tabs
        }
    }

    private void addRow() {
        int selectedTab = tabbedPane.getSelectedIndex();
        DefaultTableModel currentModel = null;

        switch (selectedTab) {
            case 0: // Rooms
                currentModel = roomTableModel;
                String roomNumber = JOptionPane.showInputDialog(frame, "Enter Room Number:");
                String roomType = JOptionPane.showInputDialog(frame, "Enter Room Type:");
                String occupiedBy = JOptionPane.showInputDialog(frame, "Enter Occupied By (leave empty if none):");
                if (roomNumber != null && roomType != null) {
                    currentModel.addRow(new Object[]{roomNumber, roomType, occupiedBy});
                    JOptionPane.showMessageDialog(frame, "Room added successfully!");
                }
                break;

            case 1: // Guests
                currentModel = guestTableModel;
                String guestName = JOptionPane.showInputDialog(frame, "Enter Guest Name:");
                String guestRoomNumber = JOptionPane.showInputDialog(frame, "Enter Room Number:");
                String stayDuration = JOptionPane.showInputDialog(frame, "Enter Stay Duration (days):");
                if (guestName != null && guestRoomNumber != null && stayDuration != null) {
                    currentModel.addRow(new Object[]{guestName, guestRoomNumber, stayDuration});
                    JOptionPane.showMessageDialog(frame, "Guest added successfully!");
                }
                break;

            case 2: // Employees
                currentModel = employeeTableModel;
                String employeeName = JOptionPane.showInputDialog(frame, "Enter Employee Name:");
                String position = JOptionPane.showInputDialog(frame, "Enter Position:");
                String responsibleFor = JOptionPane.showInputDialog(frame, "Enter Room Type Responsible For:");
                if (employeeName != null && position != null) {
                    currentModel.addRow(new Object[]{employeeName, position, responsibleFor});
                    JOptionPane.showMessageDialog(frame, "Employee added successfully!");
                }
                break;

            case 3: // Prices
                currentModel = priceTableModel;
                String serviceType = JOptionPane.showInputDialog(frame, "Enter Service Type:");
                String price = JOptionPane.showInputDialog(frame, "Enter Price:");
                if (serviceType != null && price != null) {
                    currentModel.addRow(new Object[]{serviceType, price});
                    JOptionPane.showMessageDialog(frame, "Price added successfully!");
                }
                break;

            default:
                JOptionPane.showMessageDialog(frame, "Add operation is not supported for this tab.", "Error", JOptionPane.ERROR_MESSAGE);
        }
    }

    private void deleteRow() {
        int selectedTab = tabbedPane.getSelectedIndex();
        JTable currentTable = null;
        DefaultTableModel currentModel = null;

        switch (selectedTab) {
            case 0: // Rooms
                currentTable = roomTable;
                currentModel = roomTableModel;
                break;
            case 1: // Guests
                currentTable = guestTable;
                currentModel = guestTableModel;
                break;
            case 2: // Employees
                currentTable = employeeTable;
                currentModel = employeeTableModel;
                break;
            case 3: // Prices
                currentTable = priceTable;
                currentModel = priceTableModel;
                break;
            default:
                JOptionPane.showMessageDialog(frame, "Delete operation is not supported for this tab.", "Error", JOptionPane.ERROR_MESSAGE);
                return;
        }

        if (currentTable != null && currentModel != null) {
            int selectedRow = currentTable.getSelectedRow();
            if (selectedRow != -1) {
                currentModel.removeRow(selectedRow);
                JOptionPane.showMessageDialog(frame, "Row deleted successfully!");
            } else {
                JOptionPane.showMessageDialog(frame, "No row selected!", "Error", JOptionPane.ERROR_MESSAGE);
            }
        }
    }


    
    public void updateStatistics() {
        int guestCount = guests.size();
        double totalEarnings = 0;

        for (Guest guest : guests) {
            for (Room room : rooms) {
                if (guest.getRoomNumber() == room.getRoomNumber()) {
                    for (Price price : prices) {
                        if (price.getServiceType().equals(room.getRoomType())) {
                            totalEarnings += price.getPrice() * guest.getStayDuration();
                        }
                    }
                }
            }
        }

        guestCountLabel.setText("Total Guests This Month: " + guestCount);
        earningsLabel.setText("Total Earnings: $" + String.format("%.2f", totalEarnings));
    }
}
