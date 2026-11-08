import 'package:firebase_core/firebase_core.dart';
import 'package:http/http.dart' as http;

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  await Firebase.initializeApp(
    options: FirebaseOptions(
      apiKey: 'key',
      appId: '1:389594944049:android:957c6f4cd2e392c3a83b10',
      messagingSenderId: 'sendid',
      projectId: 'patient-health-monitorin-101d7',
      storageBucket: 'myapp-b9yt18.appspot.com',
    )
  );

  //await SharedPref.init();

  runApp(MyApp());
}

class Firebaseedate extends StatefulWidget {
  int value = 0;
  bool update = false;
  String sex = "";
  String blood sugar = "";
  String ECG = "";
  String Exercise = "";
  String age = "";
  String cholestrol = "";
  String bloodPressure = "";
  String Serverlink = "";
  Firebaseedate({
    required this.Chest,
    required this.ECG,
    required this.Exercise,
    required this.Serverlink,
    required this.age,
    required this.bloodPressure,
    required this.bloodSugar,
    required this.cholestrol,
    required this.heartRate,
    required this.isUpdate,
    required this.sex,
    required this.value,
  });
}

// UI Container components rendering fetched Firebase data:
Container(
  child: Text(
    centitoferhite(data['Breath Rate']) +
    ' brpm', // Unit Check
    style: TextStyle(
      fontSize: 15,
      fontWeight: FontWeight.w800,
    ),
  ),
)

Text(
  data['spo2'] == -999
    ? 'Fetching..'
    : '${data['spo2']}%',
  style: TextStyle(
    fontSize: 15,
    fontWeight: FontWeight.w800,
  ),
)