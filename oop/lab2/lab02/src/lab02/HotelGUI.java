package lab02;

import javax.swing.*;
import java.awt.*;

/**
 * This class creates the GUI for the Hotel Guest List application.
 * It builds the interface with menus, toolbar, search panel, and table to display guest data.
 */
class GUI {
    private JFrame frame;
    private JMenuBar menuBar;
    private JMenu fileMenu, sortMenu;
    private JMenuItem openItem, saveItem, roomItem, nameItem;
    private JToolBar toolBar;
    private JButton saveButton, addButton, deleteButton;
    private JButton searchButton;
    private JComboBox<String> searchType;
    private JTextField searchField;
    private JTable dataTable;
    private JScrollPane tableScrollPane;

    /**
     * This method sets up the GUI and displays it.
     * It creates the main window with menus, toolbar, search panel, and a table for displaying guest data.
     */
    public void buildAndShowGUI() {
        // Create main frame (window) for the application
        frame = new JFrame("Hotel - Guest List");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(800, 600);

        // Create menu bar and file menu with Open and Save items
        menuBar = new JMenuBar();
        fileMenu = new JMenu("File");
        openItem = new JMenuItem("Open");
        saveItem = new JMenuItem("Save");
        fileMenu.add(openItem);
        fileMenu.add(saveItem);
        menuBar.add(fileMenu);

        // Create sort menu with sorting options: Name and Room
        sortMenu = new JMenu("Sort by");
        roomItem = new JMenuItem("Room");
        nameItem = new JMenuItem("Name");
        sortMenu.add(nameItem);
        sortMenu.add(roomItem);
        menuBar.add(sortMenu);

        // Set menu bar to the frame
        frame.setJMenuBar(menuBar);

        // Create toolbar with Save, Add, and Delete buttons
        toolBar = new JToolBar();
        saveButton = new JButton("Save");
        addButton = new JButton("Add");
        deleteButton = new JButton("Delete");
        toolBar.add(saveButton);
        toolBar.add(addButton);
        toolBar.add(deleteButton);
        frame.add(toolBar, BorderLayout.NORTH); // Add toolbar to the top (north) of the frame

        // Create a search panel with a combo box for selecting search type, text field, and search button
        JPanel searchPanel = new JPanel();
        searchType = new JComboBox<>(new String[]{"Name", "Room", "Date"}); // Dropdown for search criteria
        searchField = new JTextField(15); // Text field for inputting search terms
        searchButton = new JButton("Search");
        searchPanel.add(new JLabel("Search by:"));
        searchPanel.add(searchType);
        searchPanel.add(searchField);
        searchPanel.add(searchButton);
        frame.add(searchPanel, BorderLayout.SOUTH); // Add search panel to the bottom (south) of the frame

        // Create table to display guest data with columns for Name, Room, Check-in Date, and Check-out Date
        String[] columns = {"Name", "Room", "Check-in Date", "Check-out Date"};
        Object[][] data = {
            {"John Doe", "106", "07.10.2024", "21.10.2024"},
            {"Jane Doe", "228", "21.10.2024", "28.10.2024"}
        };
        dataTable = new JTable(data, columns); // Initialize table with data and column headers
        tableScrollPane = new JScrollPane(dataTable); // Add scroll functionality to the table
        frame.add(tableScrollPane, BorderLayout.CENTER); // Add table in the center of the frame

        // Make the frame visible
        frame.setVisible(true);
    }
}

/**
 * This is the main class that starts the Hotel Guest List application.
 * It uses SwingUtilities.invokeLater to ensure thread safety when creating the GUI.
 */
public class HotelGUI {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new GUI().buildAndShowGUI());
    }
}
