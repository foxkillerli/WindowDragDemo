package com.kaihuali.windowdragdemoandroid;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import android.view.View;
import android.os.Bundle;
import android.view.MotionEvent;

public class MainActivity extends AppCompatActivity {
    private View draggableView;
    private float dX, dY;
    private int parentWidth, parentHeight;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        draggableView = findViewById(R.id.draggableView);
        final ConstraintLayout parentLayout = findViewById(R.id.parentLayout);
        draggableView.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (parentWidth == 0 || parentHeight == 0) {
                    parentWidth = parentLayout.getWidth();
                    parentHeight = parentLayout.getHeight();
                }
                switch (motionEvent.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        dX = view.getX() - motionEvent.getRawX();
                        dY = view.getY() - motionEvent.getRawY();
                        break;

                    case MotionEvent.ACTION_MOVE:
                        float newX = motionEvent.getRawX() + dX;
                        float newY = motionEvent.getRawY() + dY;

                        // Constrain the view within the parent boundaries
                        if (newX < 0) newX = 0;
                        if (newY < 0) newY = 0;
                        if (newX + view.getWidth() > parentWidth) newX = parentWidth - view.getWidth();
                        if (newY + view.getHeight() > parentHeight) newY = parentHeight - view.getHeight();

                        view.setX(newX);
                        view.setY(newY);
                        break;

                    default:
                        return false;
                }
                return true;
            }
        });
    }
}