package xyz.sevag.animals_as_meter

import android.Manifest
import android.graphics.Bitmap
import android.graphics.Canvas
import android.os.Bundle
import android.os.Process.THREAD_PRIORITY_AUDIO
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.livinglifetechway.quickpermissions.annotations.WithPermissions
import kotlin.random.Random


class MainActivity : AppCompatActivity(), UiHelper {
    companion object {
        init {
            System.loadLibrary("animals_as_meter")
        }

        private val TAG = MainActivity::class.java.simpleName
    }

    private var streamStarted = false
    private lateinit var canvas: Canvas
    private lateinit var audioThread: Thread
    private lateinit var audioEngine: AudioEngine

    override fun onCreate(savedInstanceState: Bundle?) {
        Log.d(TAG, "onCreate: ")

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        verifyRecordPermissions()

        val bitmap = Bitmap.createBitmap(700, 1000, Bitmap.Config.ARGB_8888)
        canvas = Canvas(bitmap)

        audioEngine = AudioEngine(this)
        AudioEngine.create()
    }

    override fun onResume() {
        Log.d(TAG, "onResume: ")
        startStream()
        audioEngine.reset()
        audioThread = Thread{
            android.os.Process.setThreadPriority(THREAD_PRIORITY_AUDIO)
            audioEngine.run()
        }
        audioThread.start()
        super.onResume()
    }

    override fun displayBeat() {
        runOnUiThread{
            Log.d(TAG, "UI thread invoked")
            val r = Random.nextInt(0, 255)
            val g = Random.nextInt(0, 255)
            val b = Random.nextInt(0, 255)
            canvas.drawARGB(255, r, g, b)
        }
    }

    fun startStream() {
        if (!streamStarted) {
            AudioEngine.startRecording()
            streamStarted = true
        }
    }

    fun stopStream() {
        if (streamStarted) {
            AudioEngine.stopRecording()
            streamStarted = false
        }
    }

    override fun onPause() {
        Log.d(TAG, "onPause: ")
        audioEngine.stopLoop()
        audioThread.join()
        stopStream()
        super.onPause()
    }

    override fun onStop() {
        Log.d(TAG, "onStop: ")
        super.onStop()
    }

    override fun onDestroy() {
        Log.d(TAG, "onDestroy: ")
        super.onDestroy()
        AudioEngine.delete()
    }

    @WithPermissions(permissions = [Manifest.permission.RECORD_AUDIO])
    fun verifyRecordPermissions() {
        Toast.makeText(this, "Permissions granted", Toast.LENGTH_SHORT).show()
    }
}

