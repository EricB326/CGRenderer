using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MovingObstacle : MonoBehaviour
{
    [SerializeField] private float m_speedOfMotion;
    [SerializeField] private float m_distanceOfMotion;

    [SerializeField] private bool m_movesVertical;

    // Update is called once per frame
    void Update()
    {
        if (!m_movesVertical)
            transform.position = new Vector3(Mathf.Cos(Time.time * m_speedOfMotion) * m_distanceOfMotion, transform.position.y, transform.position.z);
        else
            transform.position = new Vector3(transform.position.x, 7.5f - (Mathf.Cos(Time.time * m_speedOfMotion) * m_distanceOfMotion), transform.position.z);
    }
}
