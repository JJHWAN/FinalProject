/*

 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.example.ndk;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.TextView;

/**
 * Snake: a simple game that everyone can enjoy.
 * 
 * This is an implementation of the classic Game "Snake", in which you control a serpent roaming
 * around the garden looking for apples. Be careful, though, because when you catch one, not only
 * will you become longer, but you'll move faster. Running into yourself or the walls will end the
 * game.
 * 
 */
public class Snake extends Activity {
    /**
     * Constants for desired direction of moving the snake
     */
    public static int MOVE_LEFT = 0;
    public static int MOVE_UP = 1;
    public static int MOVE_DOWN = 2;
    public static int MOVE_RIGHT = 3;
    
    private static String ICICLE_KEY = "snake-view";
    private SnakeView mSnakeView;

    public native int getDirFromBoard();
    public static native void updateScore(long score);
    public static native void msgBoard(int flag);
    public static native int openDevice();
    public static native void closeDevice();

    class BackThread extends Thread{
    	// waiting for interrput to wait thread
    	Handler sHandler;
    	
    	BackThread(Handler handler){
    		sHandler = handler;
    	}
    	
    	public void run(){
    		int dir;
    		
    		while(true){
    			dir = getDirFromBoard();
    			KeyEvent key = new KeyEvent(dir, 0);
    			
    			
    			switch(dir){
    			case 0:
    			dir = KeyEvent.KEYCODE_DPAD_UP;
    			break;
    			case 1:
        			dir = KeyEvent.KEYCODE_DPAD_RIGHT;
        			break;
    			case 2:
        			dir = KeyEvent.KEYCODE_DPAD_DOWN;
        			break;
    			case 3:
        			dir = KeyEvent.KEYCODE_DPAD_LEFT;
        			break;
    			}
    			onKeyDown(dir, key);
    		}
    	}
    }
    
    BackThread mThread;
    Handler mHandler = new Handler();
    
    int mThreadval = 0;
    /**
     * Called when Activity is first created. Turns off the title bar, sets up the content views,
     * and fires up the SnakeView.
     * 
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.snake_layout);
        System.loadLibrary("ndk-exam");
        int openResult = openDevice();
        if(openResult < 0){
        	System.out.println("something wrong happend while opening");
        }
        
        if(mThreadval == 0){
        	mThread = new BackThread(mHandler);
        	mThread.setDaemon(true);
        	mThread.start();
        	mThreadval++;
        }

        msgBoard(0);
        mSnakeView = (SnakeView) findViewById(R.id.snake);
        mSnakeView.setDependentViews((TextView) findViewById(R.id.text),
                findViewById(R.id.arrowContainer), findViewById(R.id.background));

        if (savedInstanceState == null) {
            // We were just launched -- set up a new game
            mSnakeView.setMode(SnakeView.READY);
        } else {
            // We are being restored
            Bundle map = savedInstanceState.getBundle(ICICLE_KEY);
            if (map != null) {
                mSnakeView.restoreState(map);
            } else {
                mSnakeView.setMode(SnakeView.PAUSE);
            }
        }
        
        // function called when screen touch 
        mSnakeView.setOnTouchListener(new OnTouchListener() {
        	// change directions to the touched point
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (mSnakeView.getGameState() == SnakeView.RUNNING) {
                    // Normalize x,y between 0 and 1
                    float x = event.getX() / v.getWidth();
                    float y = event.getY() / v.getHeight();

                    // Direction will be [0,1,2,3] depending on quadrant
                    int direction = 0;
                    direction = (x > y) ? 1 : 0;
                    direction |= (x > 1 - y) ? 2 : 0;

                    // Direction is same as the quadrant which was clicked
                    mSnakeView.moveSnake(direction);

                } else {
                    // If the game is not running then on touching any part of the screen
                    // we start the game by sending MOVE_UP signal to SnakeView
                    mSnakeView.moveSnake(MOVE_UP);
                }
                return false;
            }
        });
    }

    @Override
    protected void onPause() {
        super.onPause();
        // Pause the game along with the activity
        mSnakeView.setMode(SnakeView.PAUSE);
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        // Store the game state
        outState.putBundle(ICICLE_KEY, mSnakeView.saveState());
    }

    /**
     * Handles key events in the game. Update the direction our snake is traveling based on the
     * DPAD.
     *
     */
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent msg) {
        switch (keyCode) {
            case KeyEvent.KEYCODE_DPAD_UP:
                mSnakeView.moveSnake(MOVE_UP);
                break;
            case KeyEvent.KEYCODE_DPAD_RIGHT:
                mSnakeView.moveSnake(MOVE_RIGHT);
                break;
            case KeyEvent.KEYCODE_DPAD_DOWN:
                mSnakeView.moveSnake(MOVE_DOWN);
                break;
            case KeyEvent.KEYCODE_DPAD_LEFT:
                mSnakeView.moveSnake(MOVE_LEFT);
                break;
        }

        return super.onKeyDown(keyCode, msg);
    }
    
    @Override
    public void onDestroy(){
    	super.onDestroy();
    	msgBoard(2);
    }
}
