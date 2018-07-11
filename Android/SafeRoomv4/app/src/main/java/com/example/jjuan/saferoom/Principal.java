package com.example.jjuan.saferoom;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;

import static com.example.jjuan.saferoom.R.*;

public class Principal extends AppCompatActivity {

    private Button btnSalir;
    private Button btnConectar;
    private Button btnCreditos;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(layout.activity_principal);

        btnConectar = (Button) findViewById(id.btnConectar);
        btnConectar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent nuevaVentana= new Intent(Principal.this, DispositivosBT.class);
                startActivity(nuevaVentana);
            }
        });

        btnCreditos = (Button) findViewById(id.btnCreditos);
        btnCreditos.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                Intent nuevaVentana= new Intent(Principal.this, Creditos.class);
                startActivity(nuevaVentana);
            }
        });

        btnSalir = (Button) findViewById(id.btnSalir);
        btnSalir.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                finish();
                Intent intent = new Intent(Intent.ACTION_MAIN);
                intent.addCategory(Intent.CATEGORY_HOME);
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                startActivity(intent);
            }
        });
    }
}
