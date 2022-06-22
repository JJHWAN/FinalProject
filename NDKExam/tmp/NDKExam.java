package org.example.ndk;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.widget.TextView;

public class NDKExam extends Activity {
	
	static int x = 42;
	static int y = 1000;
	public native int add(int x, int y);
	public native int mul();
	public native void testString(String str);
	@Override
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView tv = new TextView(this);
        System.loadLibrary("ndk-exam");
 
        int z = mul();
        tv.setText("The mul of " + x + " and " + y + " is " + z);
        setContentView(tv);

        testString("test");
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
