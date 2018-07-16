struct MatrixConfig{
    int width;
    int height;
    int mode = 0;

    MatrixConfig(int width, int height) {
        this->width = width;
        this->height = height;
    }
};

struct Neighbour {
    int current;
    int up;
    int left;
    int right;
    int down;

    int row;
    int column;

    bool isFirstRow(int row) {
        return row == 0;
    }

    bool isFirstColumn(int column) {
        return column == 0;
    }
};

class PositionCalculator {
    MatrixConfig config = MatrixConfig(0,0);

    public:

    PositionCalculator(MatrixConfig config) {
        this->config = config;
    }

    Neighbour translate(int row, int column) {
        Neighbour out;

        out.row = row;
        out.column = column;

        // serpentine mode
        if (row % 2 == 0) {
            out.current = row * this->config.width + column;
            out.left = out.current - 1;
            out.up = out.current - (2 * column) - 1;
        } else {
            out.current = (row + 1) * this->config.width - column - 1;
            out.left = out.current + 1;
            out.up = (row - 1) * this->config.width + column;
        }

        return out;
    }
};