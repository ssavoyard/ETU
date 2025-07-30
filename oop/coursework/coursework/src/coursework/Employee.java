package coursework;

public class Employee {
    private String name;
    private String position;
    private String roomType;

    public Employee(String name, String position, String roomType) {
        this.name = name;
        this.position = position;
        this.roomType = roomType;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPosition() {
        return position;
    }

    public void setPosition(String position) {
        this.position = position;
    }

    public String getRoomType() {
        return roomType;
    }

    public void setRoomType(String roomType) {
        this.roomType = roomType;
    }
}