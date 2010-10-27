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

package org.efimov.tomboedit;

import android.app.Activity;
import android.content.ContentValues;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

/**
 * An activity that will edit the title of a note. Displays a floating
 * window with a text field.
 */
public class TextEntry extends Activity implements View.OnClickListener {

    /**
     * This is a special intent action that means "edit the title of a note".
     */
    public static final String TEXT_ENTRY_ACTION = "org.efimov.tomboedit.action.TEXT_ENTRY";

    /**
     * The EditText field from our UI. Keep track of this so we can extract the
     * text when we are finished.
     */
    private EditText mText;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.text_entry);

        // Get the uri of the note whose title we want to edit
        //mUri = getIntent().getData();


        // Set up click handlers for the text field and button
        mText = (EditText) this.findViewById(R.id.text);
        mText.setOnClickListener(this);
        
        Button b = (Button) findViewById(R.id.ok);
        b.setOnClickListener(this);
    }

    /*
    @Override
    protected void onResume() {
        super.onResume();

        // Initialize the text with the title column from the cursor
            // mText.setText(mCursor.getString(COLUMN_INDEX_TITLE));
    }

    @Override
    protected void onPause() {
        super.onPause();

            //ContentValues values = new ContentValues();
            //values.put(Notes.TITLE, mText.getText().toString());
            //getContentResolver().update(mUri, values, null, null);
    }
    */

    public void onClick(View v) {
        // When the user clicks, just finish this activity.
        // onPause will be called, and we save our data there.
    	Intent retIntent = new Intent();
    	retIntent.putExtra("pwd", mText.getText().toString());
    	this.setResult(0, retIntent);
        finish();
    }
}
