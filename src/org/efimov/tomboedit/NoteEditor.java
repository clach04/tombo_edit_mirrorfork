package org.efimov.tomboedit;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.content.Context;
import android.content.Intent;

import java.io.*;

import android.app.Activity;
import android.content.ComponentName;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Canvas;
import android.graphics.Paint; 
import android.graphics.Rect;
import android.net.Uri;
import android.os.Bundle;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.widget.EditText;
import android.view.GestureDetector;


/**
 * A generic activity for editing a note in a database.  This can be used
 * either to simply view a note {@link Intent#ACTION_VIEW}, view and edit a note
 * {@link Intent#ACTION_EDIT}, or create a new note {@link Intent#ACTION_INSERT}.  
 */
public class NoteEditor extends Activity {
    private static final String TAG = "TomboEdit";

    /**
     * Standard projection for the interesting columns of a normal note.
     */
    /*
    private static final String[] PROJECTION = new String[] {
            Notes._ID, // 0
            Notes.NOTE, // 1
    };
    */
    /** The index of the note column */
    private static final int COLUMN_INDEX_NOTE = 1;
    
    // This is our state data that is stored when freezing.
    private static final String ORIGINAL_CONTENT = "origContent";

    // Identifiers for our menu items.
    private static final int REVERT_ID = Menu.FIRST;
    private static final int DISCARD_ID = Menu.FIRST + 1;
    private static final int DELETE_ID = Menu.FIRST + 2;
    private static final int SETTINGS_ID = Menu.FIRST + 3;

    // The different distinct states the activity can be run in.
    private static final int STATE_EDIT = 0;
    private static final int STATE_INSERT = 1;

    private int mState;
    private boolean mNoteOnly = false;
    private Uri mUri;
    private Cursor mCursor;
    private EditText mText;
    private String mOriginalContent;
    private String mPassword;

    // Gesture 
    private GestureDetector mGestureDetector; 

    // Swipe 
    private static final int SWIPE_MIN_DISTANCE = 200; 
    private static final int SWIPE_MAX_OFF_PATH = 250; 
    private static final int SWIPE_THRESHOLD_VELOCITY = 1000; 
    
    private class Gesture extends GestureDetector.SimpleOnGestureListener { 
    	@ Override 
    	public boolean onFling (MotionEvent e1, MotionEvent e2, 
    			float velocityX,     	float velocityY) 
    	{ 
    	super.onFling (e1, e2, velocityX, velocityY); 

    	if (	e1.getX () - e2.getX ()> SWIPE_MIN_DISTANCE 
    			&& Math.abs (velocityX)> SWIPE_THRESHOLD_VELOCITY) 
    	{
    		// next page 
    	} else if (e2.getX () - e1.getX ()> SWIPE_MIN_DISTANCE) 
    	{ 
    	} 


    	return true; 
    	}     
    }
    	
    @Override 
    public boolean dispatchTouchEvent (MotionEvent ev) 
    { 
    	super.dispatchTouchEvent (ev); 
    	return mGestureDetector.onTouchEvent (ev); 
//    	return true; 
    }     	

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final Intent intent = getIntent();
        
        
        // String s = stringFromJNI();

        // Do some setup based on the action being performed.

        final String action = intent.getAction();
        if (Intent.ACTION_MAIN.equals(action)) {
            // Requested to edit: set that state, and the data being edited.
            mState = STATE_INSERT;
            File f = new File("/my/test/untitled.txt");

	        mUri = Uri.fromFile(f);
	        
	        //String path = mUri.getPath();
	        
	        //Log.e(TAG, mUri.toString());
	        //Log.e(TAG, path);

            //mUri = "ttt";
        } else if (Intent.ACTION_EDIT.equals(action) ||
        		   Intent.ACTION_VIEW.equals(action)) {
            // Requested to edit: set that state, and the data being edited.
            mState = STATE_EDIT;
            mUri = intent.getData();
            
            Log.e(TAG, mUri.getLastPathSegment());
            if(mUri.getLastPathSegment().endsWith("chi"))
            {
            	Intent intent2 = new Intent(this, TextEntry.class);
		        // Launch activity to enter password
		        this.startActivityForResult(intent2, 0);
	        }
            
        } else if (Intent.ACTION_INSERT.equals(action)) {
            // Requested to insert: set that state, and create a new entry
            // in the container.
            mState = STATE_INSERT;
            mUri = getContentResolver().insert(intent.getData(), null);

            // If we were unable to create a new note, then just finish
            // this activity.  A RESULT_CANCELED will be sent back to the
            // original activity if they requested a result.
            if (mUri == null) {
                Log.e(TAG, "Failed to insert new note into " + getIntent().getData());
                finish();
                return;
            }

            // The new entry was created, so assume all will end well and
            // set the result to be returned.
            setResult(RESULT_OK, (new Intent()).setAction(mUri.toString()));

        } else {
            // Whoops, unknown action!  Bail.
            Log.e(TAG, "Unknown action, exiting");
            finish();
            return;
        }

        // Gesture 
        mGestureDetector = new GestureDetector (this, new Gesture ());

        // Set the layout for this activity.  You can find it in res/layout/note_editor.xml
        setContentView(R.layout.note_editor);
        
        // The text view for our note, identified by its ID in the XML file.
        mText = (EditText) findViewById(R.id.note);

        // If an instance of this activity had previously stopped, we can
        // get the original text it started with.
        if (savedInstanceState != null) {
            mOriginalContent = savedInstanceState.getString(ORIGINAL_CONTENT);
        }
    }
    
    protected String ReadFileFromUri(Uri uri)
    {
    	String ret = "";
    	
		try {
			Log.e(TAG, "Reading File "+ uri.getPath());
	    	File f = new File(uri.getPath());
	    	
	        byte[] buffer = new byte[(int) f.length()];
	        BufferedInputStream fs = null;
	        try {
	            fs = new BufferedInputStream(new FileInputStream(f));
	            fs.read(buffer);
	        } finally {
	            if (fs != null) try { fs.close(); } catch (IOException ignored) { }
	        }
	        
	        int bufLen = buffer.length;
	        //Log.e(TAG, "Password is " + mPassword);
	        if(null != mPassword)
	        {
	            byte[] cipherText = buffer;
	            byte[] plainText = new byte[] { };
	            String password = mPassword;

	            boolean a = decrypt( cipherText, plainText, password);
	            
	            for(int i = 0; i < cipherText.length; i++)
	            {
	            	if('\0' == cipherText[i])
	            	{
	            		// Log.e(TAG, "BufLen is " + bufLen.toString());
	            		bufLen = i+1;
	            		break;
	            	}
	            }
	            
	        }
	        
	        String codePage = Prefs.getCodePage(getApplicationContext());
	        
	        ret = new String(buffer, 0, bufLen, codePage); // "Cp1251");
	        ret = ret.replaceAll("\r", "");
	        
	        return ret;
	        
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}    	
		
		//Log.e(TAG, "Ret="+ret);
		
		return ret;
    	
    }
    
 // Listen for results.
    protected void onActivityResult(int requestCode, int resultCode, Intent data){
        // See which child activity is calling us back.
        switch (requestCode) {
            case 0:
                // This is the standard resultCode that is sent back if the
                // activity crashed or didn't doesn't supply an explicit result.
                if (resultCode == 0 && null != data)
                {
                	mPassword = data.getStringExtra("pwd");
                } 
            default:
                break;
        }
    }
    

    @Override
    protected void onResume() {
        super.onResume();

        // If we didn't have any trouble retrieving the data, it is now
        // time to get at the stuff.
//        if (mCursor != null) {
            // Make sure we are at the one and only row in the cursor.
//            mCursor.moveToFirst();

            // Modify our overall title depending on the mode we are running in.
        /*
            if (mState == STATE_EDIT) {
                setTitle(getText(R.string.title_edit));
            } else if (mState == STATE_INSERT) {
                setTitle(getText(R.string.title_create));
            }
*/
            // This is a little tricky: we may be resumed after previously being
            // paused/stopped.  We want to put the new text in the text view,
            // but leave the user where they were (retain the cursor position
            // etc).  This version of setText does that for us.
//            String note = mCursor.getString(COLUMN_INDEX_NOTE);
//            String note = ReadFileFromUri(mUri);
//            mText.setTextKeepState(note);
            
        if(!mUri.getLastPathSegment().endsWith("chi") || null != mPassword)
        {
            // Get the note!
            String note = ReadFileFromUri(mUri);
            mText.setTextKeepState(note);
            
            // set the title
            setTitle(mUri.getLastPathSegment());
            
            // If we hadn't previously retrieved the original text, do so
            // now.  This allows the user to revert their changes.
            if (mOriginalContent == null) {
                mOriginalContent = note;
            }
        }

//        } else {
//            setTitle(getText(R.string.error_title));
//            mText.setText(getText(R.string.error_message));
//        }
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        // Save away the original text, so we still have it if the activity
        // needs to be killed while paused.
        outState.putString(ORIGINAL_CONTENT, mOriginalContent);
    }

    @Override
    protected void onPause() {
        super.onPause();

        // The user is going somewhere else, so make sure their current
        // changes are safely saved away in the provider.  We don't need
        // to do this if only editing.
        if (mCursor != null) {
            String text = mText.getText().toString();
            int length = text.length();

            // If this activity is finished, and there is no text, then we
            // do something a little special: simply delete the note entry.
            // Note that we do this both for editing and inserting...  it
            // would be reasonable to only do it when inserting.
            if (isFinishing() && (length == 0) && !mNoteOnly) {
                setResult(RESULT_CANCELED);
                deleteNote();

            // Get out updates into the provider.
            } else {
                ContentValues values = new ContentValues();

                // This stuff is only done when working with a full-fledged note.
                if (!mNoteOnly) {
                    // Bump the modification time to now.
//                    values.put(Notes.MODIFIED_DATE, System.currentTimeMillis());

                    // If we are creating a new note, then we want to also create
                    // an initial title for it.
                    if (mState == STATE_INSERT) {
                        String title = text.substring(0, Math.min(30, length));
                        if (length > 30) {
                            int lastSpace = title.lastIndexOf(' ');
                            if (lastSpace > 0) {
                                title = title.substring(0, lastSpace);
                            }
                        }
//                        values.put(Notes.TITLE, title);
                    }
                }

                // Write our text back into the provider.
//                values.put(Notes.NOTE, text);

                // Commit all of our changes to persistent storage. When the update completes
                // the content provider will notify the cursor of the change, which will
                // cause the UI to be updated.
                getContentResolver().update(mUri, values, null, null);
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
        
        menu.add(0, SETTINGS_ID, 0, R.string.menu_settings);
        

        // Build the menus that are shown when editing.
        if (mState == STATE_EDIT) {
            menu.add(0, REVERT_ID, 0, R.string.menu_revert)
                    .setShortcut('0', 'r')
                    .setIcon(android.R.drawable.ic_menu_revert);
            if (!mNoteOnly) {
                menu.add(0, DELETE_ID, 0, R.string.menu_delete)
                        .setShortcut('1', 'd')
                        .setIcon(android.R.drawable.ic_menu_delete);
            }

        // Build the menus that are shown when inserting.
        } else {
            menu.add(0, DISCARD_ID, 0, R.string.menu_discard)
                    .setShortcut('0', 'd')
                    .setIcon(android.R.drawable.ic_menu_delete);
        }

        // If we are working on a full note, then append to the
        // menu items for any other activities that can do stuff with it
        // as well.  This does a query on the system for any activities that
        // implement the ALTERNATIVE_ACTION for our data, adding a menu item
        // for each one that is found.
        if (!mNoteOnly) {
            Intent intent = new Intent(null, getIntent().getData());
            intent.addCategory(Intent.CATEGORY_ALTERNATIVE);
            menu.addIntentOptions(Menu.CATEGORY_ALTERNATIVE, 0, 0,
                    new ComponentName(this, NoteEditor.class), null, intent, 0, null);
        }

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle all of the possible menu actions.
        switch (item.getItemId()) {
        case DELETE_ID:
            deleteNote();
            finish();
            break;
        case DISCARD_ID:
            cancelNote();
            break;
        case REVERT_ID:
            cancelNote();
            break;
        case SETTINGS_ID:
            startActivity(new Intent(this, Prefs.class));
            break;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * Take care of canceling work on a note.  Deletes the note if we
     * had created it, otherwise reverts to the original text.
     */
    private final void cancelNote() {
        if (mCursor != null) {
            if (mState == STATE_EDIT) {
                // Put the original note text back into the database
                mCursor.close();
                mCursor = null;
                ContentValues values = new ContentValues();
//                values.put(Notes.NOTE, mOriginalContent);
                getContentResolver().update(mUri, values, null, null);
            } else if (mState == STATE_INSERT) {
                // We inserted an empty note, make sure to delete it
                deleteNote();
            }
        }
        setResult(RESULT_CANCELED);
        finish();
    }

    /**
     * Take care of deleting a note.  Simply deletes the entry.
     */
    private final void deleteNote() {
        if (mCursor != null) {
            mCursor.close();
            mCursor = null;
            getContentResolver().delete(mUri, null, null);
            mText.setText("");
        }
    }
    
    public native boolean decrypt( byte[] cipherText, byte[] plainText, String password);

    /* this is used to load the 'tombo-crypt' library on application
     * startup. The library has already been unpacked into
     * /data/data/org.efimov.tomboedit/lib/tombo-crypt.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("tombo-crypt");
    }    
        
}
